#![feature(path_add_extension)]
use glob::Pattern;
use std::{borrow::Cow, ffi::OsStr, fs, path::Path, process};

use serde::{Deserialize, Serialize};

pub mod consts {
    pub const BUILD_OUT_DIR: &str = "__annasul_build";
    pub const OUT_ASSET_FILE_NAME: &str = "asset.rs";
    pub const OUT_ASSET_FILE_PATH: &str = "__annasul_build/asset.rs";
}

#[macro_export]
macro_rules! build {
    () => {
        use std::path::Path;
        let base_path = Path::new(&env!("CARGO_MANIFEST_DIR"));
        let out_path = base_path.join($crate::consts::OUT_ASSET_FILE_PATH);
        let asset_dir = base_path.join("asset");
        $crate::build(&base_path, &out_path, &asset_dir)?;
    };
}

type Result<T> = std::result::Result<T, Error>;

#[derive(Debug)]
pub enum Error {
    Io(std::io::Error),
    StripPrefixError(std::path::StripPrefixError),
    Toml(toml::de::Error),
    ExtendBehaivorNotFound(String),
    NotIdentifier(String),
}

impl std::fmt::Display for Error {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        match self {
            Error::Io(e) => write!(f, "io error: {}", e),
            Error::StripPrefixError(e) => write!(f, "path strip prefix error: {}", e),
            Error::Toml(e) => write!(f, "toml deserialization error: {}", e),
            Error::ExtendBehaivorNotFound(s) => {
                write!(f, "extend behavior is not found for: {}", s)
            }
            Error::NotIdentifier(s) => write!(f, "not an identifier: {}", s),
        }
    }
}

impl std::error::Error for Error {
    fn source(&self) -> Option<&(dyn std::error::Error + 'static)> {
        match self {
            Error::Io(e) => Some(e),
            Error::StripPrefixError(e) => Some(e),
            Error::Toml(e) => Some(e),
            Error::ExtendBehaivorNotFound(_) => None,
            Error::NotIdentifier(_) => None,
        }
    }
}

impl From<std::io::Error> for Error {
    fn from(e: std::io::Error) -> Self {
        Self::Io(e)
    }
}

impl From<std::path::StripPrefixError> for Error {
    fn from(e: std::path::StripPrefixError) -> Self {
        Self::StripPrefixError(e)
    }
}

impl From<toml::de::Error> for Error {
    fn from(e: toml::de::Error) -> Self {
        Self::Toml(e)
    }
}

#[derive(Clone, Debug, Serialize, Deserialize)]
pub struct Meta {
    annasul: String,
    file: Option<MetaFile>,
    dir: Option<MetaDir>,
}

#[derive(Clone, Debug, Serialize, Deserialize)]
pub struct MetaFile {
    id: String,
    #[serde(rename = "type")]
    _type: MetaFileType,
    behavior: MetaBehavior,
}

impl MetaFile {
    pub fn with_parent(self, parent: &MetaDirFile) -> Self {
        Self {
            id: self.id.clone(),
            _type: match self._type {
                MetaFileType::Extend => parent._type.clone(),
                t => t,
            },
            behavior: match self.behavior {
                MetaBehavior::Extend => parent.behavior.clone(),
                b => b,
            },
        }
    }
}

#[derive(Clone, Debug, Serialize, Deserialize)]
pub struct MetaDir {
    id: String,
    #[serde(rename = "type")]
    _type: MetaDirType,
    file: MetaDirFile,
    filter: Option<DirFilter>,
}

#[derive(Clone, Debug, Serialize, Deserialize)]
pub struct DirFilter {
    include: Vec<String>,
    exclude: Vec<String>,
    recursive: bool,
}

impl DirFilter {
    pub fn create_buf(&self) -> DirFilterBuf {
        DirFilterBuf {
            include: self
                .include
                .iter()
                .map(|s| Pattern::new(s).unwrap())
                .collect(),
            exclude: self
                .exclude
                .iter()
                .map(|s| Pattern::new(s).unwrap())
                .collect(),
        }
    }
}

impl Default for DirFilter {
    fn default() -> Self {
        Self {
            include: vec![],
            exclude: vec![],
            recursive: true,
        }
    }
}

#[derive(Clone, Debug)]
pub struct DirFilterBuf {
    include: Vec<Pattern>,
    exclude: Vec<Pattern>,
}

impl DirFilterBuf {
    pub fn should_include(&self, path: &Path) -> bool {
        let file_name = path
            .file_name()
            .and_then(|n| n.to_str())
            .unwrap_or_default();

        // 排除模式优先
        if self.exclude.iter().any(|p| p.matches(file_name)) {
            return false;
        }

        // 包含模式检查
        self.include.is_empty() || self.include.iter().any(|p| p.matches(file_name))
    }
}

#[derive(Clone, Copy, Debug, Serialize, Deserialize)]
pub enum MetaFileType {
    #[serde(rename = "extend")]
    Extend,
    #[serde(rename = "text")]
    Text,
    #[serde(rename = "binary")]
    Binary,
}

#[derive(Clone, Copy, Debug, Serialize, Deserialize)]
pub enum MetaDirType {
    #[serde(rename = "dir")]
    Dir,
}

#[derive(Clone, Copy, Debug, Serialize, Deserialize)]
pub struct MetaDirFile {
    #[serde(rename = "type")]
    _type: MetaFileType,
    behavior: MetaBehavior,
}

impl MetaDirFile {
    pub fn with_parent(self, parent: &Self) -> Self {
        Self {
            _type: match self._type {
                MetaFileType::Extend => parent._type.clone(),
                t => t,
            },
            behavior: match self.behavior {
                MetaBehavior::Extend => parent.behavior.clone(),
                b => b,
            },
        }
    }
}

impl Default for MetaDirFile {
    fn default() -> Self {
        Self {
            _type: MetaFileType::Extend,
            behavior: MetaBehavior::Extend,
        }
    }
}

#[derive(Clone, Copy, Debug, Serialize, Deserialize)]
pub enum MetaBehavior {
    #[serde(rename = "extend")]
    Extend,
    #[serde(rename = "copy_only")]
    CopyOnly,
}

pub fn build(base_path: &Path, out_path: &Path, asset_dir: &Path) -> Result<()> {
    if !asset_dir.exists() {
        panic!("asset directory not found: {}", asset_dir.display());
    }
    println!("cargo:rerun-if-changed={}", asset_dir.display());
    println!(
        "cargo:rustc-env=ANNASUL_BUILD_ASSET_FILE_PATH={}",
        out_path.display()
    );
    if let Some(parent) = out_path.parent() {
        println!("cargo:rustc-env=ANNASUL_BUILD_OUT_DIR={}", parent.display());
        if !parent.exists() {
            fs::create_dir_all(parent)?;
        }
    }
    let mut asset = String::new();
    process_dir(base_path, asset_dir, &mut asset, &MetaDirFile::default())?;
    fs::write(&out_path, asset)?;
    match process::Command::new("rustfmt")
        .arg(format!("{}", out_path.display()))
        .spawn()
    {
        Ok(mut child) => {
            if !child.wait().unwrap().success() {
                panic!("rustfmt failed");
            }
        }
        Err(e) => {
            println!("cargo:warning=rustfmt not found: {}", e);
        }
    }
    Ok(())
}

fn process_dir(
    base_path: &Path,
    current_path: &Path,
    code: &mut String,
    meta_dir_file: &MetaDirFile,
) -> Result<()> {
    let meta_path = current_path.with_added_extension("meta");
    if !meta_path.exists() {
        panic!("meta file not found for: {}", meta_path.display());
    }

    let meta_content = fs::read_to_string(&meta_path)?;
    let meta: Meta = toml::from_str(&meta_content)?;
    assert_eq!(meta.annasul, env!("CARGO_PKG_VERSION"));
    let meta_dir = &meta.dir.expect(&format!(
        "{} is a directory, but no [dir] is defined in meta file",
        current_path.display()
    ));

    let dir_filter: Cow<DirFilter> = if let Some(filter) = &meta_dir.filter {
        Cow::Borrowed(filter)
    } else {
        Cow::Owned(DirFilter::default())
    };
    let dir_filter_buf = dir_filter.create_buf();
    let mod_name = check_ident(&meta_dir.id)?;

    code.push_str(&format!(
        "#[allow(non_snake_case)]\npub mod {} {{\n",
        mod_name
    ));
    let entries = fs::read_dir(current_path)
        .unwrap()
        .filter_map(|e| e.ok())
        .collect::<Vec<_>>();

    for entry in entries.iter() {
        let path = entry.path();
        if entry.file_type()?.is_dir() {
            if !dir_filter.recursive {
                continue;
            }
            process_dir(
                base_path,
                &path,
                code,
                &meta_dir.file.with_parent(&meta_dir_file),
            )?;
        } else if entry.file_type()?.is_file() {
            if !dir_filter_buf.should_include(&path) {
                continue;
            }
            process_file(
                base_path,
                &path,
                code,
                &meta_dir.file.with_parent(&meta_dir_file),
            )?;
        } else {
            println!(
                "cargo:warning=unknown file type for: {}",
                entry.path().display()
            );
        }
    }

    code.push_str("}\n");
    Ok(())
}

fn process_file(
    base_path: &Path,
    file_path: &Path,
    code: &mut String,
    meta_dir_file: &MetaDirFile,
) -> Result<()> {
    if file_path.extension() == Some(&OsStr::new("meta")) {
        let path = file_path.with_extension("");
        if !path.exists() {
            panic!("file not found for: {}", path.display());
        }
        return Ok(());
    }
    let meta_path = file_path.with_added_extension("meta");
    if !meta_path.exists() {
        panic!("meta file not found for: {}", meta_path.display());
    }

    let meta_content = fs::read_to_string(&meta_path)?;
    let meta: Meta = toml::from_str(&meta_content)?;
    assert_eq!(meta.annasul, env!("CARGO_PKG_VERSION"));
    let meta_file = meta
        .file
        .expect(&format!(
            "{} is a file, but no [file] is defined in meta file",
            file_path.display()
        ))
        .with_parent(meta_dir_file);
    match meta_file.behavior {
        MetaBehavior::Extend => Err(Error::ExtendBehaivorNotFound(format!(
            "{}",
            file_path.display()
        ))),
        MetaBehavior::CopyOnly => {
            let relative_path = file_path
                .strip_prefix(base_path)?
                .to_str()
                .unwrap()
                .replace('\\', "/");

            let const_name = check_ident(&meta_file.id)?;

            let (ty, macro_used) = match meta_file._type {
                MetaFileType::Extend => Err(Error::ExtendBehaivorNotFound(format!(
                    "{}",
                    file_path.display()
                )))?,
                MetaFileType::Text => ("&'static str", "include_str!"),
                MetaFileType::Binary => ("&'static [u8]", "include_bytes!"),
            };

            code.push_str(&format!(
                r#"#[allow(non_upper_case_globals)]
pub const {const_name}: {ty} = {macro_used}(concat!(env!("CARGO_MANIFEST_DIR"), "/{relative_path}"));"#
));
            Ok(())
        }
    }
}

fn check_ident(name: &str) -> Result<String> {
    let mut s = String::with_capacity(name.len());
    if let Some(first) = name.chars().next() {
        if first.is_numeric() {
            Err(Error::NotIdentifier(format!(
                "{:?} is not an identifier, because it starts with a number",
                name
            )))?
        }
    }
    for c in name.chars() {
        if c.is_alphanumeric() || c == '_' {
            s.push(c);
        } else {
            Err(Error::NotIdentifier(format!(
                "{:?} is not an identifier, because it contains non-alphanumeric character: {:?}",
                name, c
            )))?;
        }
    }
    Ok(s)
}
