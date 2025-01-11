fn main() {
    if std::env::var("TARGET").unwrap().contains("ios") {
        println!("cargo:rustc-env=IPHONEOS_DEPLOYMENT_TARGET=15.0");
        println!("cargo:rustc-link-arg=-mios-version-min=15.0");
    }
} 