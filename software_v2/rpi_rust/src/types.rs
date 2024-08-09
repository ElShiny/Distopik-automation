#[derive(Debug)]
pub enum SlaveType {
    Ui,
    Akt,
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