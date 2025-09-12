sudo apt-get update
sudo apt-get install build-essential meson ninja-build xorriso qemu-system qemu-utils
sudo apt-get clean 
if command -v brew >/dev/null 2>&1; then
    echo "Homebrew is already installed, and will not be reinstalled."
else
    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
    echo >> ~/.bashrc
    echo 'eval "$(/home/linuxbrew/.linuxbrew/bin/brew shellenv)"' >> ~/.bashrc
    source ~/.bashrc
fi

curl --proto '=https' --tlsv1.2 -sSf https://sh.rustup.rs | sh

echo "Environment Setup Complete. Run `source ~/.bashrc` to update your PATH."