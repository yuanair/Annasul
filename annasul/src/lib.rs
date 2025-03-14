pub use annasul_build::consts;

#[macro_export]
macro_rules! include_asset {
    () => {
        include!(env!("ANNASUL_BUILD_ASSET_FILE_PATH"));
    };
}
