# airetzar-ng

> Automated wireless deauthentication tool built on top of the [Aircrack-ng](https://www.aircrack-ng.org/) suite.

`airetzar-ng` reads the CSV output of `airodump-ng`, groups the captured access
points by channel, and drives `aireplay-ng` deauthentication frames against
them. It hops through the channels automatically, repeats the run for a
configurable number of iterations, and can skip specific BSSIDs through a
whitelist.

## Table of contents

- [Legal notice](#legal-notice)
- [How it works](#how-it-works)
- [Requirements](#requirements)
- [Build](#build)
- [Usage](#usage)
  - [Capturing the access points](#capturing-the-access-points)
  - [Output](#output)
- [Releases](#releases)
- [Notes](#notes)
- [License](#license)

## Legal notice

This tool is intended **only** for authorized security testing, research, and
education — for example, assessing the resilience of a network you own or have
explicit written permission to test.

Deauthenticating devices from networks you do not own or are not authorized to
test is illegal in most jurisdictions and may disrupt service for others. You
are solely responsible for complying with all applicable laws and for obtaining
authorization before use. The authors and contributors assume no liability for
misuse or damage caused by this tool.

## How it works

For every iteration, and for every channel found in the capture file, the tool:

1. Switches the monitor-mode interface to that channel with `airmon-ng`.
2. Launches one `aireplay-ng -0` process per access point detected on that
   channel (whitelisted BSSIDs are skipped).
3. Waits for every process of the current channel to finish before moving on to
   the next channel.

Access points on the same channel are processed in parallel; channels are
processed one after another.

## Requirements

- Linux with a wireless adapter that supports monitor mode.
- The Aircrack-ng suite installed and available in `PATH` — specifically
  `airmon-ng` and `aireplay-ng`.
- A C++11 compiler (`g++`) to build from source.
- Root privileges, since monitor mode and frame injection require them.

## Build

Run the helper script, which compiles the sources into `build/airetzar-ng`:

```bash
./deploy.sh
```

Equivalently, compile by hand:

```bash
g++ --std=c++11 src/*.cpp -o build/airetzar-ng
```

## Usage

```bash
build/airetzar-ng -f airodump-out.csv -i mon0 -t 5 -w mac0,mac1 -ap 5
```

| Option | Required | Default | Description |
| --- | --- | --- | --- |
| `-f` | yes | — | Path to the CSV file produced by `airodump-ng`. |
| `-i` | yes | — | Monitor-mode interface used for the run. |
| `-t` | yes | `0` | Number of iterations. Each iteration replays the full channel list. |
| `-w` | no | _(none)_ | Comma-separated list of BSSIDs to skip, e.g. `84:AA:9C:C0:3D:58,8A:F7:C7:D0:E5:5C`. |
| `-ap` | no | `4` | Number of packets sent per `aireplay-ng` invocation. |

> With `-t 0` (the default) the tool parses the capture file but performs no
> iterations, so remember to pass a positive value.

### Capturing the access points

Start monitor mode and let `airodump-ng` collect nearby networks into a CSV
file:

```bash
airmon-ng start wlp2s0
airodump-ng -w myOutput --output-format csv mon0
```

Stop `airodump-ng` once enough networks have been collected, then point
`airetzar-ng` at the generated CSV file. An [`example.csv`](example.csv)
capture is included in the repository so you can try the parser without
capturing live traffic.

### Output

Each line reports the current iteration, the targeted BSSID, and its channel:

```text
09:13:54 Sending DeAuth 1/1 - BSSID: [84:AA:9C:C0:3D:58] - C: 9
09:13:55 Sending DeAuth 1/1 - BSSID: [EC:22:80:5C:B5:91] - C: 1
09:13:56 Sending DeAuth 1/1 - BSSID: [34:57:60:E1:60:D0] - C: 2
```

## Releases

Every push to `master` builds the project and creates a GitHub release tagged
`v<version>`. Each release includes the Linux x86-64 binary and its SHA-256
checksum.

The [`VERSION`](VERSION) file is the single source of truth for the release
version. Increment it using [semantic versioning](https://semver.org/) before
each push to `master`; the workflow rejects a version whose release already
exists instead of overwriting it.

## Notes

- The whitelist matches BSSIDs exactly, so values must be written in the same
  uppercase hexadecimal format used by `airodump-ng`.
- Access points are read in the order they appear in the capture file.
- Only BSSIDs (access points) are read from the capture; the station section of
  the CSV is ignored.

## License

Distributed under the terms of the GNU General Public License v3.0. See
[LICENSE](LICENSE) for the full text.
