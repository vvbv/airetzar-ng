# airetzar-ng

Automated wireless deauthentication tool built on top of the Aircrack-ng suite.

`airetzar-ng` reads the CSV output of `airodump-ng`, groups the captured access
points by channel, and sends `aireplay-ng` deauthentication frames to each one of
them. It hops through the channels automatically, repeats the attack for a
configurable number of iterations, and can skip specific BSSIDs through a
whitelist.

## How it works

For every iteration, and for every channel found in the capture file, the tool:

1. Switches the monitor-mode interface to that channel with `airmon-ng`.
2. Launches one `aireplay-ng -0` attack per access point detected on that channel.
3. Waits for every attack of the current channel to finish before moving on.

## Requirements

- Linux with a wireless adapter supporting monitor mode.
- The Aircrack-ng suite installed and available in `PATH` (`airmon-ng`,
  `aireplay-ng`).
- Root privileges, since monitor mode and frame injection require them.

## Build

```bash
./deploy.sh
```

The script compiles the sources into `build/airetzar-ng`:

```bash
g++ --std=c++11 src/*.cpp -o build/airetzar-ng
```

## Usage

```bash
build/airetzar-ng -f airodump-out.csv -t 5 -w mac0,mac1 -ap 5 -i mon0
```

| Option | Description |
| --- | --- |
| `-f` | Path to the CSV file produced by `airodump-ng -w <output> --output-format csv <interface>`. |
| `-i` | Monitor-mode interface used for the attack. |
| `-t` | Number of iterations. Each iteration replays the full channel list. |
| `-w` | Comma-separated list of BSSIDs to skip, e.g. `84:AA:9C:C0:3D:58,8A:F7:C7:D0:E5:5C`. |
| `-ap` | Number of deauthentication packets sent by each `aireplay-ng` invocation. Defaults to `4`. |

### Capturing the access points

```bash
airmon-ng start wlp2s0
airodump-ng -w myOutput --output-format csv mon0
```

Stop `airodump-ng` once enough networks have been collected, then point
`airetzar-ng` to the generated CSV file.

### Output

```text
09:13:54 Sending DeAuth 1/1 - BSSID: [84:AA:9C:C0:3D:58] - C: 9
09:13:55 Sending DeAuth 1/1 - BSSID: [EC:22:80:5C:B5:91] - C: 1
09:13:56 Sending DeAuth 1/1 - BSSID: [34:57:60:E1:60:D0] - C: 2
```

An `example.csv` capture file is included in the repository for testing.

## Notes

- The whitelist matches BSSIDs exactly, so the values must be written in the
  same uppercase hexadecimal format used by `airodump-ng`.
- Access points are attacked in the order they appear in the capture file.
- Every BSSID of a channel is attacked in parallel, one `aireplay-ng` process
  per access point.

## License

Distributed under the terms of the GNU General Public License v3.0. See
[LICENSE](LICENSE) for the full text.
