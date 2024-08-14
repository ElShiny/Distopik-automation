use crate::types::{SlaveType, Status};
use std::{thread};
use std::time::Duration;
use std::str;
use rppal::gpio::{Gpio, OutputPin, InputPin};
use serialport::SerialPortBuilder;
use tokio_modbus::prelude::*;



pub mod unirel_pot;
pub mod unirel_sw;



#[derive(Debug)]
pub enum AktuatorType {
    Undefined = 0,
    UnirelPot = 1,
    UnirelSw = 2,
    UnirelPotMini = 3,
    UnirelSwMini = 4,
}

#[derive(Debug)]
pub struct AktLine {
    pub tty_path: &'static str,
    pub baud_rate: u32,
    pub akt_builder: SerialPortBuilder,
    pub gpio_rst: OutputPin,
    pub gpio_int: InputPin,
    pub number_of_slaves: u8,
    pub slave_addresses: Vec<u8>,
    pub aktuators: Vec<Box<dyn std::any::Any>>,
}

impl AktLine {
    pub fn new(tty_path: &'static str, baud_rate: u32, gpio_rst: u8, gpio_int: u8) -> Result<AktLine, Box<dyn std::error::Error>> {

        let mut  rst_pin = Gpio::new()?.get(gpio_rst)?.into_output();
        rst_pin.set_reset_on_drop(false);
        let int_pin = Gpio::new()?.get(gpio_int)?.into_input();

        let akt_builder = tokio_serial::new(tty_path, baud_rate).flow_control(tokio_serial::FlowControl::None);

        Ok(AktLine {
            tty_path,
            baud_rate,
            akt_builder,
            gpio_rst: rst_pin,
            gpio_int: int_pin,
            number_of_slaves: 0,
            slave_addresses: Vec::new(),
            aktuators: Vec::new(),
        })
    }

    pub fn reset_modules(&mut self) {
        self.gpio_rst.set_low();
        thread::sleep(Duration::from_millis(100));
        self.gpio_rst.set_high();
        thread::sleep(Duration::from_millis(1000));
    }

    pub fn get_connected_modules_id(&mut self) {
        for i in 0..20 {
            
            let mut ctx = sync::rtu::connect_slave_with_timeout(
                &self.akt_builder,
                Slave(i),
                Some(Duration::new(0, 100000)),
            ).unwrap();
    
            let rsp = ctx.read_discrete_inputs(1, 1);
    
            if rsp.is_ok() {

                self.slave_addresses.push(i);
                self.number_of_slaves += 1;

                println!("Found module at address: {:#?}", i);
            }
            if rsp.is_err(){
                //println!("Error: {:?}", rsp.err());
            }
        }
        if self.number_of_slaves == 0 {
            println!("No modules found in address range 0-20");
        }
    }

    pub fn get_modules_type(&mut self){

        for i in 0..self.number_of_slaves {
            
            let mut ctx = sync::rtu::connect_slave_with_timeout(
                &self.akt_builder,
                Slave(self.slave_addresses[i as usize]),
                Some(Duration::new(0, 100000)),
            ).unwrap();
    
            let rsp = ctx.read_discrete_inputs(1, 8);
    
            if rsp.is_ok() {
                //convert rsp bool array to uint
                let mut res: u8 = 0;
                for j in  0..8 {
                    res |= (rsp.as_ref().unwrap()[j] as u8) << j;
                }
    
                let status = res & 0x01;
                let board_type = (res & 0x02) >> 1;
                let board_version = (res & 0xfc) >> 2;
                println!("Connected board: {:#x}, {:#x}, {:#x}", status, board_type, board_version);

                if(board_version == 1){
                    let akt_struct = unirel_pot::UnirelPot::new(self.slave_addresses[i as usize], Status::from_u8(status));
                    self.aktuators.push(Box::new(akt_struct));
                }
                else if (board_version == 2){
                    let akt_struct = unirel_sw::UnirelSW::new(self.slave_addresses[i as usize], Status::from_u8(status));
                    self.aktuators.push(Box::new(akt_struct));
                    
                }
                
                //self.aktuators.push(Box::new(akt_struct));
            }
            if rsp.is_err(){
                println!("Error: {:?}", rsp.err());
            }
        }
    }


}



