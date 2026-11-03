#!/usr/bin/env bash
# Disposable Ubuntu 24.04 runner only. Never provision a persistent test host here.
set -euo pipefail
test "$(uname -s)" = Linux
test "${RUNNER_ENVIRONMENT:-}" = github-hosted
. /etc/os-release
test "$ID" = ubuntu
test "$VERSION_ID" = 24.04
qualification_key="${RUNNER_TEMP:?}/llvm23-signing-key.asc"
curl --fail --silent --show-error --location https://apt.llvm.org/llvm-snapshot.gpg.key --output "$qualification_key"
gpg --show-keys --with-colons "$qualification_key" | awk -F: '$1 == "fpr" {print $10}' | grep -Fx 6084F3CF814B57C1CF12EFD515CF4D18AF4F7421
sudo install -m 0644 "$qualification_key" /usr/share/keyrings/uwvm-llvm23.asc
printf '%s\n' 'deb [signed-by=/usr/share/keyrings/uwvm-llvm23.asc] https://apt.llvm.org/noble/ llvm-toolchain-noble-23 main' | sudo tee /etc/apt/sources.list.d/uwvm-llvm23.list
sudo apt-get update
qualification_packages=(clang-23 lld-23 llvm-23 libc++-23-dev libc++abi-23-dev libunwind-23-dev)
for qualification_package in "${qualification_packages[@]}"; do
  qualification_candidate="$(apt-cache policy "$qualification_package" | sed -n 's/^  Candidate: //p')"
  test -n "$qualification_candidate"
  test "$qualification_candidate" != '(none)'
  printf '%s %s\n' "$qualification_package" "$qualification_candidate"
done
sudo apt-get install -y --no-install-recommends "${qualification_packages[@]}" cmake ninja-build wabt
dpkg-query -W -f='${Package}\t${Version}\t${Architecture}\n' "${qualification_packages[@]}"
apt-cache policy "${qualification_packages[@]}"
