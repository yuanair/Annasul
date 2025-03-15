use log::Level;
use std::panic::Location;

pub trait LogResultExt<T, E>: Sized {
    fn log_unwrap(self, level: Level) -> T;
    fn log_expect(self, level: Level, msg: &str) -> T;
    fn error_unwrap(self) -> T {
        self.log_unwrap(Level::Error)
    }
    fn error_expect(self, msg: &str) -> T {
        self.log_expect(Level::Error, msg)
    }
    fn warn_unwrap(self) -> T {
        self.log_unwrap(Level::Warn)
    }
    fn warn_expect(self, msg: &str) -> T {
        self.log_expect(Level::Warn, msg)
    }
    fn info_unwrap(self) -> T {
        self.log_unwrap(Level::Info)
    }
    fn info_expect(self, msg: &str) -> T {
        self.log_expect(Level::Info, msg)
    }
    fn debug_unwrap(self) -> T {
        self.log_unwrap(Level::Debug)
    }
    fn debug_expect(self, msg: &str) -> T {
        self.log_expect(Level::Debug, msg)
    }
    fn trace_unwrap(self) -> T {
        self.log_unwrap(Level::Trace)
    }
    fn trace_expect(self, msg: &str) -> T {
        self.log_expect(Level::Trace, msg)
    }
}

impl<T, E> LogResultExt<T, E> for Result<T, E>
where
    E: std::error::Error + Send + Sync + 'static,
{
    #[track_caller]
    fn log_unwrap(self, level: Level) -> T {
        match self {
            Ok(t) => t,
            Err(e) => {
                let location = Location::caller();
                log::log!(level, "at {location}: {e:?}");
                panic!("Error occurred: {e}")
            }
        }
    }
    #[track_caller]
    fn log_expect(self, level: Level, msg: &str) -> T {
        match self {
            Ok(t) => t,
            Err(e) => {
                let location = Location::caller();
                log::log!(level, "at {location}: {msg}: {e:?}");
                panic!("{msg}: {e}")
            }
        }
    }
}
