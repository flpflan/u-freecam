{
  description = " Unity freecam (Internal)";

  inputs = {
    nixpkgs.url = "flake:nixpkgs";
  };

  outputs =
    {
      nixpkgs,
      ...
    }:
    let
      eachSystem = nixpkgs.lib.genAttrs nixpkgs.lib.platforms.unix;
      pkgsFor = eachSystem (
        system:
        import nixpkgs {
          inherit system;
          config = {
            allowUnfree = true;
          };
        }
      );
    in
    {
      devShells = eachSystem (
        system:
        let
          pkgs = pkgsFor.${system};
          ndkVersion = "29.0.14206865";
          ndkBundle =
            (pkgs.androidenv.composeAndroidPackages {
              includeNDK = true;
              inherit ndkVersion;
            }).ndk-bundle;
        in
        {
          default = pkgs.mkShell {
            packages = with pkgs; [
              # bear # Use CMAKE_EXPORT_COMPILE_COMMANDS=ON
              cmake
              gnumake
              ndkBundle
              ninja
              ccache
            ];

            ANDROID_NDK = "${ndkBundle}/libexec/android-sdk/ndk/${ndkVersion}";

            shellHook = ''
              export PATH=${ndkBundle}/libexec/android-sdk/ndk/${ndkVersion}/toolchains/llvm/prebuilt/linux-x86_64/bin:$PATH
              echo -e "\033[32m Development environment loaded! \033[0m"
            '';
          };
        }
      );
    };
}
