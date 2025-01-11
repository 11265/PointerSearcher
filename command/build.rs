use std::{env, path::Path};

fn main() {
    let out_dir = env::var("OUT_DIR").unwrap();
    let target = env::var("TARGET").unwrap();
    
    // 设置库的搜索路径
    let lib_path = Path::new(&out_dir)
        .parent()
        .unwrap()
        .parent()
        .unwrap()
        .parent()
        .unwrap();
    
    println!("cargo:rustc-link-search=native={}", lib_path.display());
    
    // 根据平台配置链接选项
    if target.contains("windows") {
        println!("cargo:rustc-link-lib=static=ptrscan");
    } else if target.contains("apple") {
        println!("cargo:rustc-link-lib=static=ptrscan");
    } else {
        // Linux 平台
        println!("cargo:rustc-link-lib=static=ptrscan");
        println!("cargo:rustc-link-arg=-Wl,--allow-multiple-definition");
    }
    
    // 重新构建条件
    println!("cargo:rerun-if-changed=build.rs");
} 