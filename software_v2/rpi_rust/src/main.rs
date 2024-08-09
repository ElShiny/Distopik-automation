use akt::AktLine;
use std::time::Duration;
use tokio_serial::SerialPortBuilder;
use tokio_modbus::prelude::*;


pub mod akt;
pub mod types;

//static tty_path_ui: &'static str = "/dev/ttyAMA2";
static tty_path_akt: &str = "/dev/ttyAMA1";
static baud_rate: u32 = 921600;

const GPIO_RST_UI: u8 = 14;
const GPIO_RST_AKT: u8 = 22;

fn main() -> Result<(), Box<dyn std::error::Error>> {

    let mut akt_line = AktLine::new(tty_path_akt, baud_rate, GPIO_RST_AKT, 0)?;
    akt_line.reset_modules();
    akt_line.get_modules();
    println!("{:#?}", akt_line);

    Ok(())
}

fn check_connected_slaves(builder: &SerialPortBuilder) -> Vec<u16> {
    let mut conn: Vec<u16> = Vec::new();
    for i in 0..20 {
        
        let mut ctx = sync::rtu::connect_slave_with_timeout(
            &builder,
            Slave(i),
            Some(Duration::new(0, 100000)),
        )
        .unwrap();

        let mut rsp = ctx.read_discrete_inputs(1, 8);

        if rsp.is_ok() {
            conn.push(i as u16);
            //print response
            //println!("Sensor response: {:?}", rsp.as_ref().unwrap());
            //convert rsp bool array to uint
            let mut res: u8 = 0;
            for j in  0..8 {
                res |= (rsp.as_ref().unwrap()[j] as u8) << j;
            }

            let status = res & 0x01;
            let board_type = (res & 0x02) >> 1;
            let board_version = (res & 0xfc) >> 2;
            println!("Connected board: {:#x}, {:#x}, {:#x}", status, board_type, board_version);
            
        }
        if rsp.is_err(){
            println!("Error: {:?}", rsp.err());
        }
    }
    return conn;
}
//
//fn slave_change_address_remote(
//    builder: &SerialPortBuilder,
//    current_slave: u8,
//    new_addr: u8,
//) -> Result<(), Box<dyn std::error::Error>> {
//    //let mut ctx2 = sync::rtu::connect_slave_with_timeout(&builder2, Slave(2), Some(Duration::new(1, 0)))?;
//    let mut i: u64 = 0;
//    while true {
//        if i == 0 {
//            let mut ctx = sync::rtu::connect_slave_with_timeout(
//                &builder,
//                Slave(current_slave),
//                Some(Duration::new(1, 0)),
//            )?;
//            let rsp = ctx.read_holding_registers(999, 1);
//            if rsp.is_ok() {
//                println!("Input buffer value: {:?}", rsp.unwrap());
//                ctx.write_single_coil(100, true);
//                i = 1;
//            } else if rsp.is_err() {
//                println!("Sensor failed 1 {:?}", rsp);
//                thread::sleep(Duration::from_millis(500));
//            }
//        }
//
//        if i == 1 {
//            let mut ctx = sync::rtu::connect_slave_with_timeout(
//                &builder,
//                Slave(1),
//                Some(Duration::new(1, 0)),
//            )?;
//            thread::sleep(Duration::from_millis(500));
//            let rsp2 = ctx.write_single_register(999, new_addr as u16);
//            let rsp3 = ctx.write_single_register(1000, 1);
//
//            if rsp2.is_ok() && i == 1 {
//                println!("Input buffer value: {:?}", rsp2.unwrap());
//                i = 2;
//                //i = 0;
//            } else if rsp2.is_err() || rsp3.is_err() {
//                println!("Sensor failed 2 {:?}", rsp2);
//                println!("Sensor failed 2 {:?}", rsp3);
//                thread::sleep(Duration::from_millis(500));
//            }
//        }
//
//        if i == 2 {
//            //thread::sleep(Duration::from_millis(3000));
//            let mut ctx = sync::rtu::connect_slave_with_timeout(
//                &builder,
//                Slave(new_addr),
//                Some(Duration::new(1, 0)),
//            )?;
//            let rsp3 = ctx.read_holding_registers(999, 10);
//
//            if rsp3.is_ok() {
//                println!("Input buffer value: {:?}", rsp3.unwrap());
//                return Ok(());
//            } else if rsp3.is_err() {
//                println!("Sensor failed 3 {:?}", rsp3);
//                thread::sleep(Duration::from_millis(500));
//            }
//        }
//
//        //println!("Sensor response: {:?}", rsp);
//        //500 ms delay
//        //thread::sleep(Duration::from_millis(10));
//        //i+=1;
//    }
//    Ok(())
//}
//
//fn slave_change_address_btn(
//    builder: &SerialPortBuilder,
//    new_addr: u8,
//) -> Result<(), Box<dyn std::error::Error>> {
//    //let mut ctx2 = sync::rtu::connect_slave_with_timeout(&builder2, Slave(2), Some(Duration::new(1, 0)))?;
//    let mut i: u64 = 0;
//    while true {
//        if i == 0 {
//            let mut ctx = sync::rtu::connect_slave_with_timeout(
//                &builder,
//                Slave(1),
//                Some(Duration::new(1, 0)),
//            )?;
//            let rsp = ctx.read_holding_registers(999, 1);
//            if rsp.is_ok() {
//                println!("Input buffer value: {:?}", rsp.unwrap());
//                i = 1;
//            } else if rsp.is_err() {
//                println!("Sensor failed 1 {:?}", rsp);
//                thread::sleep(Duration::from_millis(500));
//            }
//        }
//
//        if i == 1 {
//            let mut ctx = sync::rtu::connect_slave_with_timeout(
//                &builder,
//                Slave(1),
//                Some(Duration::new(1, 0)),
//            )?;
//            thread::sleep(Duration::from_millis(500));
//            let rsp2 = ctx.write_single_register(999, new_addr as u16);
//            let rsp3 = ctx.write_single_register(1000, 1);
//
//            if rsp2.is_ok() && i == 1 {
//                println!("Input buffer value: {:?}", rsp2.unwrap());
//                i = 2;
//                //i = 0;
//            } else if rsp2.is_err() || rsp3.is_err() {
//                println!("Sensor failed 2 {:?}", rsp2);
//                println!("Sensor failed 2 {:?}", rsp3);
//                thread::sleep(Duration::from_millis(500));
//            }
//        }
//
//        if i == 2 {
//            //thread::sleep(Duration::from_millis(3000));
//            let mut ctx = sync::rtu::connect_slave_with_timeout(
//                &builder,
//                Slave(new_addr),
//                Some(Duration::new(1, 0)),
//            )?;
//            let rsp3 = ctx.read_holding_registers(999, 10);
//
//            if rsp3.is_ok() {
//                println!("Input buffer value: {:?}", rsp3.unwrap());
//                thread::sleep(Duration::from_millis(500));
//                return Ok(());
//            } else if rsp3.is_err() {
//                println!("Sensor failed 3 {:?}", rsp3);
//                thread::sleep(Duration::from_millis(500));
//            }
//        }
//
//        //println!("Sensor response: {:?}", rsp);
//        //500 ms delay
//        //thread::sleep(Duration::from_millis(10));
//        //i+=1;
//    }
//    Ok(())
//}
//