use std::fmt::Display;

#[derive(Debug)]
pub enum Error {
    InvalidParam(String),
}

impl Display for Error {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        match self {
            Error::InvalidParam(e) => write!(f, "invalid param: {}", e),
        }
    }
}

impl std::error::Error for Error {
    fn source(&self) -> Option<&(dyn std::error::Error + 'static)> {
        match self {
            Error::InvalidParam(_) => None,
        }
    }
}

pub type Result<T> = std::result::Result<T, Error>;

pub fn get_qq_avatar_url(qq_number: &str, size: u16) -> Result<String> {
    match size {
        0..=5 | 40 | 100 | 140 | 160 | 640 | 1080 => {}
        _ => return Err(Error::InvalidParam(format!("invalid size: {}", size))),
    }
    Ok(format!(
        "https://q1.qlogo.cn/g?b=qq&nk={}&s={}",
        qq_number, size
    ))
}
