use std::time::Duration;

use rppal::gpio::{Gpio, OutputPin};
use serialport::SerialPortBuilder;
use tokio::time::error::Elapsed;
use std::thread;
use std::fs::OpenOptions;
use std::io::prelude::*;
use tokio_modbus::prelude::*;

static  tty_path: &'static str = "/dev/ttyAMA1";//"/dev/ttyAMA1"
static  baud_rate: u32 = 921600;

const GPIO_RST_UI: u8 = 14;

fn main() -> Result<(), Box<dyn std::error::Error>> {
 

    //let slave = Slave(0x01);
    let mut rst_ui_pin = Gpio::new()?.get(GPIO_RST_UI)?.into_output();
    rst_ui_pin.set_reset_on_drop(false);
    //rst_ui_pin.set_bias(rppal::gpio::PinBias::PullUp);
    //rst_ui_pin.set_low();
    //thread::sleep(Duration::from_millis(100));
    rst_ui_pin.set_high();
    thread::sleep(Duration::from_millis(1000));

    let builder = tokio_serial::new(tty_path, baud_rate).flow_control(tokio_serial::FlowControl::None);
    //let builder2 = tokio_serial::new(tty_path, baud_rate).flow_control(tokio_serial::FlowControl::None);
    //let mut ctx = sync::rtu::connect_slave_with_timeout(&builder, slave, Some(Duration::new(1, 0)))?;
    //let mut ctx = sync::rtu::connect_slave(&builder, slave)?;

    if check_connected_slaves(&builder) == 1 {
        slave_change_address_btn(&builder, 15)?;
    }
    write_slave_holding_buffer1(&builder, 15)?;
    //write_slave_holding_buffer(&builder, 11)?;
    //write_slave_holding_buffer(&builder, 12)?;
    //write_slave_holding_buffer(&builder, 13)?;
    read_slave_input_buffer(&builder, 15, 500, 10)?;


//read_slave_holding_buffer(&builder, 10, 1000, 10)?;

    //read_slave_coils(&builder, 10)?;
    //write_slave_coils(&builder, 10)?;
    //read_slave_coils(&builder, 10)?;

    //read_slave_discrete_inputs(&builder, 15)?;
    //check_slave_repeatedly(&builder, 16)?;
    //slave_change_address_remote(&builder, 12, 10)?;
 
    Ok(())
}

fn check_connected_slaves(builder: &SerialPortBuilder) -> u16{
    for i in 0..20 {
        let mut ctx = sync::rtu::connect_slave_with_timeout(&builder, Slave(i), Some(Duration::new(0, 100000))).unwrap();
        let rsp = ctx.read_coils(100, 1);
        //println!("Sensor value is: {} {:?}", i, rsp);
        if rsp.is_ok() {
            println!("Sensor found at address: {}", i);
            if(i == 1){return 1;}
        }
        else if rsp.is_err() {
            println!("Sensor failed at: {}", i);   
        }
        
    }
    return 0;
}
fn check_slave_repeatedly(builder: &SerialPortBuilder, slave: u8) -> Result<(), Box<dyn std::error::Error>>{
    let mut ctx = sync::rtu::connect_slave_with_timeout(&builder, Slave(slave), Some(Duration::new(1, 0)))?;
    let mut i: u64 = 0;
    while true {
        
        let rsp = ctx.read_coils(100, 1);
        if rsp.is_err() {
            println!("Sensor failed at: {}", i);   
        }
        println!("Sensor response: {:?}", rsp);        
        //500 ms delay
        thread::sleep(Duration::from_millis(1));
        i+=1;
    }
    Ok(())
}

fn slave_change_address_remote(builder: &SerialPortBuilder, current_slave: u8, new_addr: u8) -> Result<(), Box<dyn std::error::Error>>{
    
    //let mut ctx2 = sync::rtu::connect_slave_with_timeout(&builder2, Slave(2), Some(Duration::new(1, 0)))?;
    let mut i: u64 = 0;
    while true {


        if i == 0{
            let mut ctx = sync::rtu::connect_slave_with_timeout(&builder, Slave(current_slave), Some(Duration::new(1, 0)))?;
            let rsp = ctx.read_holding_registers(999, 1);
            if rsp.is_ok() {
                println!("Input buffer value: {:?}", rsp.unwrap());
                ctx.write_single_coil(100, true);
                i = 1;
            }
            else if rsp.is_err() {
                println!("Sensor failed 1 {:?}", rsp);    
                thread::sleep(Duration::from_millis(500));
            }

        }
        
        if i == 1{
            let mut ctx = sync::rtu::connect_slave_with_timeout(&builder, Slave(1), Some(Duration::new(1, 0)))?;
            thread::sleep(Duration::from_millis(500));
            let rsp2 = ctx.write_single_register(999, new_addr as u16);
            let rsp3 = ctx.write_single_register(1000, 1);
            
            if rsp2.is_ok() &&i==1 {
                println!("Input buffer value: {:?}", rsp2.unwrap());
                i=2;
                //i = 0;
            }
            else if rsp2.is_err() || rsp3.is_err() {
                println!("Sensor failed 2 {:?}", rsp2);    
                println!("Sensor failed 2 {:?}", rsp3);    
                thread::sleep(Duration::from_millis(500));
            }
        
        }

        if i == 2{
            //thread::sleep(Duration::from_millis(3000));
            let mut ctx = sync::rtu::connect_slave_with_timeout(&builder, Slave(new_addr), Some(Duration::new(1, 0)))?;
            let rsp3 = ctx.read_holding_registers(999, 10);
            
            if rsp3.is_ok(){
                println!("Input buffer value: {:?}", rsp3.unwrap());
                return Ok(());
            }
            else if rsp3.is_err() {
                println!("Sensor failed 3 {:?}", rsp3);    
                thread::sleep(Duration::from_millis(500));
            }
        }



        //println!("Sensor response: {:?}", rsp);
        //500 ms delay
        //thread::sleep(Duration::from_millis(10));
        //i+=1;
    }
    Ok(())
}

fn slave_change_address_btn(builder: &SerialPortBuilder, new_addr: u8) -> Result<(), Box<dyn std::error::Error>>{
    
    //let mut ctx2 = sync::rtu::connect_slave_with_timeout(&builder2, Slave(2), Some(Duration::new(1, 0)))?;
    let mut i: u64 = 0;
    while true {


        if i == 0{
            let mut ctx = sync::rtu::connect_slave_with_timeout(&builder, Slave(1), Some(Duration::new(1, 0)))?;
            let rsp = ctx.read_holding_registers(999, 1);
            if rsp.is_ok() {
                println!("Input buffer value: {:?}", rsp.unwrap());
                i = 1;
            }
            else if rsp.is_err() {
                println!("Sensor failed 1 {:?}", rsp);    
                thread::sleep(Duration::from_millis(500));
            }

        }
        
        if i == 1{
            let mut ctx = sync::rtu::connect_slave_with_timeout(&builder, Slave(1), Some(Duration::new(1, 0)))?;
            thread::sleep(Duration::from_millis(500));
            let rsp2 = ctx.write_single_register(999, new_addr as u16);
            let rsp3 = ctx.write_single_register(1000, 1);
            
            if rsp2.is_ok() &&i==1 {
                println!("Input buffer value: {:?}", rsp2.unwrap());
                i=2;
                //i = 0;
            }
            else if rsp2.is_err() || rsp3.is_err() {
                println!("Sensor failed 2 {:?}", rsp2);    
                println!("Sensor failed 2 {:?}", rsp3);    
                thread::sleep(Duration::from_millis(500));
            }
        
        }

        if i == 2{
            //thread::sleep(Duration::from_millis(3000));
            let mut ctx = sync::rtu::connect_slave_with_timeout(&builder, Slave(new_addr), Some(Duration::new(1, 0)))?;
            let rsp3 = ctx.read_holding_registers(999, 10);
            
            if rsp3.is_ok(){
                println!("Input buffer value: {:?}", rsp3.unwrap());
                thread::sleep(Duration::from_millis(500));
                return Ok(());
            }
            else if rsp3.is_err() {
                println!("Sensor failed 3 {:?}", rsp3);    
                thread::sleep(Duration::from_millis(500));
            }
        }



        //println!("Sensor response: {:?}", rsp);
        //500 ms delay
        //thread::sleep(Duration::from_millis(10));
        //i+=1;
    }
    Ok(())
}

fn read_slave_holding_buffer(builder: &SerialPortBuilder, slave: u8, addr: u16, length: u16) -> Result<(), Box<dyn std::error::Error>>{
    let mut ctx = sync::rtu::connect_slave_with_timeout(&builder, Slave(slave), Some(Duration::new(1, 0)))?;
    let mut i: u64 = 0;

        let rsp = ctx.read_holding_registers(addr, length);
        if rsp.is_ok() {
            println!("Input buffer value: {:?}", rsp.unwrap());
        }
        else if rsp.is_err() {
            println!("Sensor failed {:?}", rsp);   
            thread::sleep(Duration::from_millis(500));
        }
        //println!("Sensor response: {:?}", rsp);
        //500 ms delay
        //thread::sleep(Duration::from_millis(10));
        i+=1;

    Ok(())
}

fn read_slave_coils(builder: &SerialPortBuilder, slave: u8) -> Result<(), Box<dyn std::error::Error>>{
    let mut ctx = sync::rtu::connect_slave_with_timeout(&builder, Slave(slave), Some(Duration::new(2, 0)))?;

    let rsp = ctx.read_coils(100, 16);
    if rsp.is_ok() {
        println!("Input buffer value: {:?}", rsp.unwrap());
    }
    else if rsp.is_err() {
        println!("Sensor failed {:?}", rsp);   
        //thread::sleep(Duration::from_millis(500));
    }


    Ok(())
}

fn write_slave_coils(builder: &SerialPortBuilder, slave: u8) -> Result<(), Box<dyn std::error::Error>>{
    let mut ctx = sync::rtu::connect_slave_with_timeout(&builder, Slave(slave), Some(Duration::new(2, 0)))?;

    let rsp = ctx.write_multiple_coils(100, &[true]);
    if rsp.is_ok() {
        println!("Input buffer value: {:?}", rsp.unwrap());
    }
    else if rsp.is_err() {
        println!("Sensor failed {:?}", rsp);   
        //thread::sleep(Duration::from_millis(500));
    }


    Ok(())
}

fn read_slave_discrete_inputs(builder: &SerialPortBuilder, slave: u8) -> Result<(), Box<dyn std::error::Error>>{
    let mut ctx = sync::rtu::connect_slave_with_timeout(&builder, Slave(slave), Some(Duration::new(2, 0)))?;

    let rsp = ctx.read_discrete_inputs(1, 8);
    if rsp.is_ok() {
        println!("Input buffer value: {:?}", rsp.unwrap());
    }
    else if rsp.is_err() {
        println!("Sensor failed {:?}", rsp);   
        //thread::sleep(Duration::from_millis(500));
    }

    Ok(())
}

fn read_slave_input_buffer(builder: &SerialPortBuilder, slave: u8, addr: u16, length: u16) -> Result<(), Box<dyn std::error::Error>>{
    let mut ctx = sync::rtu::connect_slave_with_timeout(&builder, Slave(slave), Some(Duration::new(1, 0)))?;
    let mut i: u64 = 0;
    while true{
        let rsp = ctx.read_input_registers(addr, length);
        if rsp.is_ok() {
            let vec_i16: Vec<i16> = rsp.unwrap().iter().map(|&x| x as i16).collect();
            println!("Input buffer value {i}: {:?}", vec_i16);
            //thread::sleep(Duration::from_millis(100));
        }
        else if rsp.is_err() {
            println!("Sensor failed {:?}", rsp);   
            //thread::sleep(Duration::from_millis(500));
        }
        i+=1;
    }

    Ok(())
}

fn write_slave_holding_buffer(builder: &SerialPortBuilder, slave: u8) -> Result<(), Box<dyn std::error::Error>>{
    let mut ctx = sync::rtu::connect_slave_with_timeout(&builder, Slave(slave), Some(Duration::new(2, 0)))?;
    let rsp = ctx.write_multiple_registers(999 + 30, &[0,255,255, 50,0,0]);
    let rsp = ctx.write_multiple_registers(999 + 22, &[0, 360]);
    let rsp = ctx.write_multiple_registers(999 + 20, &[2]);
    let rsp = ctx.write_multiple_registers(999 + 24, &[2]);

    let rsp = ctx.write_multiple_registers(999 + 6, &[0, 360]);
    let rsp = ctx.write_multiple_registers(999 + 2, &[0]);
    let rsp = ctx.write_multiple_registers(999 + 8, &[100]);
    let rsp = ctx.write_single_register(1000, 1);
    let rsp = ctx.read_holding_registers(1000, 10);
        
    if rsp.is_ok() {
        println!("Input buffer value: {:?}", rsp.unwrap());
        //thread::sleep(Duration::from_millis(50));
    }
    else if rsp.is_err() {
        println!("Sensor failed {:?}", rsp);   
        //thread::sleep(Duration::from_millis(500));
    }

    Ok(())
}
fn write_slave_holding_buffer1(builder: &SerialPortBuilder, slave: u8) -> Result<(), Box<dyn std::error::Error>>{
    let mut ctx = sync::rtu::connect_slave_with_timeout(&builder, Slave(slave), Some(Duration::new(2, 0)))?;
    let rsp = ctx.write_multiple_registers(999 + 2, &[2100]);
    let rsp = ctx.write_single_register(1000, 1);


        
    if rsp.is_ok() {
        println!("Input buffer value: {:?}", rsp.unwrap());
        //thread::sleep(Duration::from_millis(50));
    }
    else if rsp.is_err() {
        println!("Sensor failed {:?}", rsp);   
        //thread::sleep(Duration::from_millis(500));
    }

    Ok(())
}