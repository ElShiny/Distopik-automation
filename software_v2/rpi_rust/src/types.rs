#[derive(Debug)]
pub enum SlaveType {
    Ui,
    Akt,
    None,
}

#[derive(Debug)]
pub enum Status {
    Working,
    NotWorking,
}

#[derive(Debug)]
pub struct Slave {
    pub address: u8,
    pub slave_type: SlaveType,
    pub status: Status,
}

impl SlaveType {
    pub fn from_u8(value: u8) -> SlaveType {
        match value {
            1 => SlaveType::Ui,
            2 => SlaveType::Akt,
            _ => SlaveType::None,
        }
    }
}

impl Status {
    pub fn from_u8(value: u8) -> Status {
        match value {
            0 => Status::NotWorking,
            1 => Status::Working,
            _ => Status::NotWorking,
        }
    }
    
}