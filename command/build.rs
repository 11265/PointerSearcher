use std::{env, path::Path};

fn main() {
    let out_dir = env::var("OUT_DIR").unwrap();
    let target = env::var("TARGET").unwrap();
    
    // 获取工作空间根目录
    let workspace_root = Path::new(&out_dir)
        .parent()
        .unwrap()
        .parent()
        .unwrap()
        .parent()
        .unwrap();
    
    // 设置库的搜索路径 - 添加多个可能的路径
    println!("cargo:rustc-link-search=native={}", workspace_root.display());
    println!("cargo:rustc-link-search=native={}/target/debug", workspace_root.display());
    println!("cargo:rustc-link-search=native={}/target/release", workspace_root.display());
    
    if let Ok(target_dir) = env::var("CARGO_TARGET_DIR") {
        println!("cargo:rustc-link-search=native={}", target_dir);
    }
    
    // 根据平台配置链接选项
    if target.contains("windows") {
        println!("cargo:rustc-link-lib=dylib=ptrscan");
    } else if target.contains("apple") {
        println!("cargo:rustc-link-lib=dylib=ptrscan");
    } else {
        // Linux 平台
        println!("cargo:rustc-link-lib=dylib=ptrscan");
        println!("cargo:rustc-link-arg=-Wl,--allow-multiple-definition");
    }
    
    // 重新构建条件
    println!("cargo:rerun-if-changed=build.rs");
} 