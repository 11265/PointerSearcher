fn main() {
    if std::env::var("TARGET").unwrap().contains("ios") {
        println!("cargo:rustc-env=IPHONEOS_DEPLOYMENT_TARGET=15.0");
        println!("cargo:rustc-link-arg=-mios-version-min=15.0");
        println!("cargo:rustc-link-arg=-target");
        println!("cargo:rustc-link-arg=arm64-apple-ios15.0");
        
        let sdk_path = std::process::Command::new("xcrun")
            .args(["--sdk", "iphoneos", "--show-sdk-path"])
            .output()
            .expect("failed to execute xcrun")
            .stdout;
        let sdk_path = String::from_utf8_lossy(&sdk_path).trim().to_string();
        
        println!("cargo:rustc-link-arg=-isysroot");
        println!("cargo:rustc-link-arg={}", sdk_path);
        
        println!("cargo:rustc-env=CFLAGS=-target arm64-apple-ios15.0 -isysroot {} -mios-version-min=15.0", sdk_path);
        println!("cargo:rustc-env=CXXFLAGS=-target arm64-apple-ios15.0 -isysroot {} -mios-version-min=15.0", sdk_path);
    }
} 