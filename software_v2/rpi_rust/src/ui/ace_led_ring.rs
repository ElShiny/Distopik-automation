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

//ACE SETTINGS LOCATION
const ACE_MODE: u16 = 2;
const ACE_DISABLE: u16 = 3;
const ACE_INCOMING_VALUE: u16 = 4;
const ACE_INCOMING_VALUE_CHANGE: u16 = 5;
const ACE_START_ANGLE: u16 = 6;
const ACE_STOP_ANGLE: u16 = 7;
const ACE_SCALE: u16 = 8;

//LED SETTINGS LOCATION
const LED_MODE: u16 = 20;
const LED_DISABLE: u16 = 21;
const LED_START_ANGLE: u16 = 22;
const LED_STOP_ANGLE: u16 = 23;
const LED_SWITCH_STEPS: u16 = 24;
const LED_FRONT_COLOR: u16 = 30;
const LED_BACK_COLOR: u16 = 33;
const LED_ARRAY: u16 = 40;


pub trait AceLedRingTrait {
    //fn new(akt_address: u8, akt_status: Status) -> Self;
    fn read_settings(&self);
    fn read_module_settings(&mut self, akt_builder: &SerialPortBuilder,);
    fn read_relay_return_vals(&mut self, akt_builder: &SerialPortBuilder);
    fn write_relay_values(&mut self, akt_builder:& SerialPortBuilder);
}



#[derive(Debug)]
pub struct AceLedRing {
    slave_type: SlaveType,
    ui_address: u8,
    ui_status: Status,

}

impl AceLedRingTrait for AceLedRing {

    fn read_settings(&self){
        println!("slave_type: {:#x?}", self.slave_type);
        println!("ui_address: {:#x?}", self.ui_address);
        println!("ui_status: {:#x?}", self.ui_status);
    }

    fn read_module_settings(&mut self, akt_builder:& SerialPortBuilder){
        let mut ctx = sync::rtu::connect_slave_with_timeout(
            akt_builder,
            Slave(self.ui_address as u8),
            Some(Duration::new(0, 100000)),
        ).unwrap();

        let rsp = ctx.read_holding_registers(HOLDING_REGISTERS_START, 30);

        if rsp.is_ok() {
            let rsp = rsp.unwrap();
        }
        let rsp = ctx.read_input_registers(INPUT_REGISTERS_START, 11);
        if rsp.is_ok() {
            let rsp = rsp.unwrap();
        }

    }

    fn read_relay_return_vals(&mut self, akt_builder:& SerialPortBuilder){
        let mut ctx = sync::rtu::connect_slave_with_timeout(
            akt_builder,
            Slave(self.ui_address as u8),
            Some(Duration::new(0, 100000)),
        ).unwrap();

        let rsp = ctx.read_input_registers(INPUT_REGISTERS_START, 11);
        if rsp.is_ok() {
            let rsp = rsp.unwrap();
        }
    }
    fn write_relay_values(&mut self, akt_builder:& SerialPortBuilder){
        let mut ctx = sync::rtu::connect_slave_with_timeout(
            akt_builder,
            Slave(self.ui_address as u8),
            Some(Duration::new(1, 0)),
        ).unwrap();       

    }
}

impl AceLedRing{
    pub fn new(akt_address: u8, akt_status: Status) -> AceLedRing{
        AceLedRing {
            slave_type: SlaveType::Ui,
            ui_address: akt_address,
            ui_status: akt_status,
        }
    }
}