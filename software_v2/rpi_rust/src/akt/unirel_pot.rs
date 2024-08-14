use crate::types::{SlaveType, Status};
use serialport::SerialPortBuilder;
use tokio_modbus::prelude::*;
use std::time::Duration;


const INPUT_REGISTERS_START: u16 = 500;
const INPUT_REGISTERS_NREGS: u16 = 150;

const HOLDING_REGISTERS_START: u16 = 999;
const HOLDING_REGISTERS_NREGS: u16 = 201;

const COILS_START: u16 = 100;
const COILS_NREGS: u16 = 10;

const DISCRETE_INPUTS_START: u16 = 1;
const DISCRETE_INPUTS_NREGS: u16 = 20;

const MSPS_CONNECTED: u16 = 0;
const RELAY_RET_VALUES: u16 = 1;

const SETTINGS_CHANGED: u16 = 1;

const RELAY_MCPS_EN: u16 = 2;
const RELAY_LINES_EN: u16 = 3;
const RELAY_MASK: u16 = 10;
const RELAY_VALUES: u16 = 20;

pub trait UnirelPotTrait {
    //fn new(akt_address: u8, akt_status: Status) -> Self;
    fn read_settings(&self);
    fn read_module_settings(&mut self, akt_builder: &SerialPortBuilder,);
    fn read_relay_return_vals(&mut self, akt_builder: &SerialPortBuilder);
    fn write_relay_values(&mut self, akt_builder:& SerialPortBuilder);
}



#[derive(Debug)]
pub struct UnirelPot {
    slave_type: SlaveType,
    akt_address: u8,
    akt_status: Status,

    mcps_connected: u16,
    relay_ret_values: [u16; 10],

    pub relay_mcps_en: u16,
    pub relay_lines_en: u16,
    pub relay_mask: [u16; 10],
    pub relay_values: [u16; 10],
}

impl UnirelPotTrait for UnirelPot {

    fn read_settings(&self){
        println!("akt_address: {:#x?}", self.akt_address);
        println!("mcps_connected: {:#x?}", self.mcps_connected);
        println!("relay_ret_values: {:?}", self.relay_ret_values);
        println!("relay_mcps_en: {:#x?}", self.relay_mcps_en);
        println!("relay_lines_en: {:#x?}", self.relay_lines_en);
        println!("relay_mask: {:?}", self.relay_mask);
        println!("relay_values: {:?}", self.relay_values);
    }

    fn read_module_settings(&mut self, akt_builder:& SerialPortBuilder){
        let mut ctx = sync::rtu::connect_slave_with_timeout(
            akt_builder,
            Slave(self.akt_address as u8),
            Some(Duration::new(0, 100000)),
        ).unwrap();

        let rsp = ctx.read_holding_registers(HOLDING_REGISTERS_START, 30);

        if rsp.is_ok() {
            let rsp = rsp.unwrap();


            self.relay_mcps_en = rsp[RELAY_MCPS_EN as usize];
            self.relay_lines_en = rsp[RELAY_LINES_EN as usize];
            for i in 0..10 {
                self.relay_mask[i] = rsp[i+RELAY_MASK as usize];
            }
            for i in 0..10 {
                self.relay_values[i] = rsp[i+RELAY_VALUES as usize];
            }
        }
        let rsp = ctx.read_input_registers(INPUT_REGISTERS_START, 11);
        if rsp.is_ok() {
            let rsp = rsp.unwrap();
            self.mcps_connected = rsp[MSPS_CONNECTED as usize];
            for i in 0..10 {
                self.relay_ret_values[i] = rsp[i+RELAY_RET_VALUES as usize];
            }
        }

    }

    fn read_relay_return_vals(&mut self, akt_builder:& SerialPortBuilder){
        let mut ctx = sync::rtu::connect_slave_with_timeout(
            akt_builder,
            Slave(self.akt_address as u8),
            Some(Duration::new(0, 100000)),
        ).unwrap();

        let rsp = ctx.read_input_registers(INPUT_REGISTERS_START, 11);
        if rsp.is_ok() {
            let rsp = rsp.unwrap();
            self.mcps_connected = rsp[MSPS_CONNECTED as usize];
            for i in 0..10 {
                self.relay_ret_values[i] = rsp[i+RELAY_RET_VALUES as usize];
            }
        }
    }
    fn write_relay_values(&mut self, akt_builder:& SerialPortBuilder){
        let mut ctx = sync::rtu::connect_slave_with_timeout(
            akt_builder,
            Slave(self.akt_address as u8),
            Some(Duration::new(1, 0)),
        ).unwrap();

        let rsp = ctx.write_multiple_registers(HOLDING_REGISTERS_START + RELAY_VALUES, &self.relay_values[0..1]);
        let rsp = ctx.write_multiple_registers(HOLDING_REGISTERS_START + SETTINGS_CHANGED, &[1]);
        if rsp.is_ok() {
            println!("Relay values written");
        }
        if rsp.is_err() {
            println!("Error writing relay values");
        }
        

    }
}

impl UnirelPot{
    pub fn new(akt_address: u8, akt_status: Status) -> UnirelPot{
        UnirelPot {
            slave_type: SlaveType::Akt,
            akt_address: akt_address,
            akt_status: akt_status,

            mcps_connected: 0,
            relay_ret_values: [0; 10],

            relay_mcps_en: 0,
            relay_lines_en: 0,
            relay_mask: [0; 10],
            relay_values: [0; 10],
        }
    }
}