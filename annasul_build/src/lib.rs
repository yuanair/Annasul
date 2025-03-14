#![feature(path_add_extension)]
use std::{ffi::OsStr, fs, path::Path};

use serde::{Deserialize, Serialize};


#[macro_export]
macro_rules! build {
    () => {
        use std::path::Path;
        let base_path = Path::new(&env!("CARGO_MANIFEST_DIR"));
        let out_path = base_path.join("src/__asset.rs");
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
}

impl std::fmt::Display for Error {
    fn fmt(&self, f: &mut std::fmt::Formatter) -> std::fmt::Result {
        match self {
            Error::Io(e) => write!(f, "io error: {}", e),
            Error::StripPrefixError(e) => write!(f, "path strip prefix error: {}", e),
            Error::Toml(e) => write!(f, "toml deserialization error: {}", e),
            Error::ExtendBehaivorNotFound(s) => write!(f, "extend behavior is not found for: {}", s),
        }
    }
}

impl std::error::Error for Error {
    fn source(&self) -> Option<&(dyn std::error::Error +'static)> {
        match self {
            Error::Io(e) => Some(e),
            Error::StripPrefixError(e) => Some(e),
            Error::Toml(e) => Some(e),
            Error::ExtendBehaivorNotFound(_) => None,
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
            }
        }
    }
}

#[derive(Clone, Debug, Serialize, Deserialize)]
pub struct MetaDir {
    id: String,
    #[serde(rename = "type")]
    _type: MetaDirType,
    file: MetaDirFile,
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
            }
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
    let mut asset = String::new();
    process_dir(base_path, asset_dir, &mut asset, &MetaDirFile::default())?;
    fs::write(&out_path, asset)?;
    Ok(())
}

fn process_dir(base_path: &Path, current_path: &Path, code: &mut String, meta_dir_file: &MetaDirFile) -> Result<()> {
    let entries = fs::read_dir(current_path)
        .unwrap()
        .filter_map(|e| e.ok())
        .collect::<Vec<_>>();

    for entry in entries.iter().filter(|e| e.path().is_dir()) {
        let dir_name = entry.path();
        let meta_path = dir_name.with_added_extension("meta");
        if !meta_path.exists() {
            println!("cargo:warning=meta file not found for: {:?}", meta_path);
            continue;
        }

        let meta_content = fs::read_to_string(&meta_path)?;
        let meta: Meta = toml::from_str(&meta_content)?;
        assert_eq!(meta.annasul, env!("CARGO_PKG_VERSION"));
        let meta_dir = &meta.dir.expect(&format!("{:?} is a directory, but no [dir] is defined in meta file", dir_name));
        let mod_name = sanitize_ident(&meta_dir.id);
        
        code.push_str(&format!(
            "#[allow(non_snake_case)]\npub mod {} {{\n",
            mod_name
        ));
        process_dir(base_path, &entry.path(), code, &meta_dir.file.with_parent(&meta_dir_file))?;
        code.push_str("}\n");
    }

    for entry in entries.iter().filter(|e| e.path().is_file()) {
        let file_path = entry.path();
        if file_path.extension() == Some(&OsStr::new("meta")) {
            continue;
        }
        let meta_path = file_path.with_added_extension("meta");
        if !meta_path.exists() {
            println!("cargo:warning=meta file not found for: {:?}", meta_path);
            continue;
        }

        let meta_content = fs::read_to_string(&meta_path)?;
        let meta: Meta = toml::from_str(&meta_content)?;
        assert_eq!(meta.annasul, env!("CARGO_PKG_VERSION"));
        let meta_file = meta.file.expect(&format!("{:?} is a file, but no [file] is defined in meta file", file_path)).with_parent(meta_dir_file);
        match meta_file.behavior {
            MetaBehavior::Extend => Err(Error::ExtendBehaivorNotFound(format!("{}", file_path.display())))?,
            MetaBehavior::CopyOnly => {

                let relative_path = file_path
                    .strip_prefix(base_path)?
                    .to_str()
                    .unwrap()
                    .replace('\\', "/");
        
                let const_name = sanitize_ident(&meta_file.id);
        
                let (ty, macro_used) = match meta_file._type {
                    MetaFileType::Extend => Err(Error::ExtendBehaivorNotFound(format!("{}", file_path.display())))?,
                    MetaFileType::Text => ("&'static str", "include_str!"),
                    MetaFileType::Binary => ("&'static [u8]", "include_bytes!"),
                };
        
                code.push_str(&format!(
                    r#"#[allow(non_upper_case_globals)]
pub const {const_name}: {ty} = {macro_used}(concat!(env!("CARGO_MANIFEST_DIR"), "/{relative_path}"));"#
));
            }
        }
        
    }
    Ok(())
}

fn sanitize_ident(name: &str) -> String {
    let mut s = String::with_capacity(name.len());
    for c in name.chars() {
        if c.is_alphanumeric() {
            s.push(c);
        } else {
            s.push('_');
        }
    }
    
    if let Some(first) = s.chars().next() {
        if first.is_numeric() {
            s.insert(0, '_');
        }
    }
    
    s
}
