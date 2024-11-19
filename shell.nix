{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = with pkgs; [
    cmake
    boost
    openssl
    git
    clang
    ninja
  ];

  shellHook = ''
    # Set CC and CXX to use Clang
    export CC=${pkgs.clang}/bin/clang
    export CXX=${pkgs.clang}/bin/clang++
  '';
}
