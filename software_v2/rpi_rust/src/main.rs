use std::time::Duration;

use rppal::gpio::{Gpio, OutputPin};
use std::thread;
use std::fs::OpenOptions;
use std::io::prelude::*;

fn main() -> Result<(), Box<dyn std::error::Error>> {
    use tokio_modbus::prelude::*;

    let tty_path = "/dev/ttyAMA1";
    let baud_rate = 921600;
    let slave = Slave(0x01);


    let builder = tokio_serial::new(tty_path, baud_rate).flow_control(tokio_serial::FlowControl::None);

    //let mut ctx = sync::rtu::connect_slave_with_timeout(&builder, slave, Some(Duration::new(1, 0)))?;
    let mut ctx = sync::rtu::connect_slave(&builder, slave)?;
    ctx.set_timeout(Some(Duration::new(0, 10000)));
    println!("Reading a sensor value");

    for i in 0..20 {
        let rsp = ctx.read_input_registers(999, 4)?;
        println!("Sensor value is: {} {:?}", i, rsp);
        //delay
        thread::sleep(Duration::from_millis(50));
    }

 
    Ok(())
}