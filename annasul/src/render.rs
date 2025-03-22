//!
//! pseudo-code:
//! trait Surface:
//!
//! trait Renderer:
//!     create_surface(&self, window: &Window) -> Result<Surface>
//! struct RendererBuilder:
//!     new() -> Self
//!     set_api(&mut self, api: RenderAPI) -> &mut Self
//!     build(&self) -> Result<Self>
//!
//!
//!

///
/// # Render Error
///
#[derive(Debug)]
pub enum Error {
    APILoadingError(Box<dyn std::error::Error>),
    EnumeratePhysicalDevicesError(Box<dyn std::error::Error>),
    NoSupportedPhysicalDeviceFound,
    DeviceCreationError(Box<dyn std::error::Error>),
    CreateSurfaceError(Box<dyn std::error::Error>),
}

impl std::fmt::Display for Error {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        match self {
            Error::APILoadingError(e) => write!(f, "API loading error: {}", e),
            Error::EnumeratePhysicalDevicesError(e) => {
                write!(f, "Enumerate physical devices error: {}", e)
            }
            Error::NoSupportedPhysicalDeviceFound => {
                write!(f, "No supported physical device found")
            }
            Error::DeviceCreationError(e) => write!(f, "Device creation error: {}", e),
            Error::CreateSurfaceError(e) => write!(f, "Create surface error: {}", e),
        }
    }
}

impl std::error::Error for Error {}

///
/// # Render Result
///
pub type Result<T> = std::result::Result<T, Error>;

///
/// # Surface
///
pub trait Surface {}

///
/// # Renderer
///
pub trait Renderer {
    ///
    /// # Create Surface
    ///
    /// Creates a surface for the given window.
    ///
    /// # Arguments
    /// + `window`: A reference to the window to create the surface for.
    ///
    fn create_surface(&self, window: &winit::window::Window) -> Result<Box<dyn Surface>>;
}

///
/// # Render API
///
#[derive(Default, Debug, Clone, Copy)]
pub enum RenderAPI {
    #[default]
    WGPU,
}

///
/// # Renderer Builder
///
#[derive(Default)]
pub struct RendererBuilder {
    api: RenderAPI,
}

impl RendererBuilder {
    ///
    /// # Default
    /// + `api`: `RenderAPI::Vulkan`
    ///
    pub fn new() -> Self {
        Self {
            api: RenderAPI::WGPU,
        }
    }

    ///
    /// # Set API
    ///
    pub fn set_api(&mut self, api: RenderAPI) -> &mut Self {
        self.api = api;
        self
    }

    ///
    /// # Build
    ///
    pub fn build(&self) -> Result<Box<dyn Renderer>> {
        match self.api {
            RenderAPI::WGPU => Ok(todo!()),
        }
    }
}
