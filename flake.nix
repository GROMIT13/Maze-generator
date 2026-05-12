{
  description = "Maze generator";

  inputs = {
    nixpkgs.url = "github:NixOs/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = {
    self,
    nixpkgs,
    flake-utils,
  }:
    flake-utils.lib.eachDefaultSystem (
      system: let
        pkgs = import nixpkgs {inherit system;};
      in {
        devShells.default = pkgs.mkShell {
          buildInputs = with pkgs; [
            cmake
            gnumake
            gcc
            raylib

            xorg.libX11
            xorg.libXcursor
            xorg.libXi
            xorg.libXinerama
            xorg.libXrandr
            libGL
          ];

          shellHook = ''
            export LD_LIBRARY_PATH="{pkgs.lib.makeLibraryPath [ pkgs.libGL pkgs.xorg.libX11 ]}:$LD_LIBRARY_PATH"
            echo "Maze-generator ready to run and develop."
            echo "To compile the project run 'cmake -B build && cmake --build build'"
          '';
        };
      }
    );
}
