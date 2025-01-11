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
    
    // 设置库的搜索路径
    println!("cargo:rustc-link-search=native={}/target/{}/debug", workspace_root.display(), target);
    println!("cargo:rustc-link-search=native={}/target/{}/release", workspace_root.display(), target);
    
    // 使用静态链接
    println!("cargo:rustc-link-lib=static=ptrscan");
    
    // iOS 平台特殊处理
    if target.contains("ios") {
        println!("cargo:rustc-link-lib=framework=Foundation");
        println!("cargo:rustc-link-lib=framework=Security");
        println!("cargo:rustc-link-lib=framework=UIKit");
    } else if !target.contains("windows") {
        println!("cargo:rustc-link-arg=-Wl,--allow-multiple-definition");
    }
    
    // 重新构建条件
    println!("cargo:rerun-if-changed=build.rs");
} 