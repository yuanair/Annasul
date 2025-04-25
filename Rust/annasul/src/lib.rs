///
/// 应用
///
#[cfg(feature = "application")]
pub mod application;
///
/// 日志
///
#[cfg(feature = "log")]
pub mod log;
///
/// 预导入
///
pub mod prelude;
///
/// 渲染
///
#[cfg(feature = "render")]
pub mod render;
///
/// 工具
///
pub mod utils;
///
/// 窗口管理
///
#[cfg(feature = "window")]
pub mod window;

/// # Include the generated asset file.
/// ```json
/// // .vscode/settings.json
/// // Attention: If you get UNRESOLVED_ENV_VAR error in vscode rust-analyzer, please add the following configuration in .vscode/settings.json:
/// {
///     // ...
///     "rust-analyzer.cargo.extraEnv": {
///         "BINDING_PATH": "${workspaceFolder}/example/build.rs" // your build.rs path
///     },
///     "rust-analyzer.check.extraEnv": {
///         "BINDING_PATH": "${workspaceFolder}/example/build.rs" // your build.rs path
///     }
///     // ...
/// }
/// ```
#[macro_export]
macro_rules! include_asset {
    () => {
        include!(env!("ANNASUL_BUILD_ASSET_FILE_PATH"));
    };
}
