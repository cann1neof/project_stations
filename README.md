# Project Stations

A small console simulator of a public bus network, written in C++.

You describe stations, bus lines and the number of buses in a plain-text config file. The program then:

- simulates buses driving back and forth along their lines, one tick at a time;
- finds the fastest route between any two stations with Dijkstra's algorithm, including transfers between lines;
- estimates the total travel time, counting how long you wait for the nearest bus and how long the ride takes.

## Quick start

The code includes `<bits/stdc++.h>`, so it needs **GCC** (Linux, WSL, or Homebrew `g++-NN` on macOS). Apple Clang won't compile it.

```bash
g++ -std=c++17 main.cpp -o main
```

```bash
./main
```

Run it from the repository root: the config path `./example.cfe` is hard-coded in `main.cpp`.

## Using the program

After start-up the program waits for input:

| You type | What happens |
| --- | --- |
| anything except `y` (e.g. `n`) | The simulation advances one tick. Each bus prints its state: time left at the stop (`aboard`) or distance to the next stop (`distance`). |
| `y` | You're asked for the start and end station IDs (e.g. `0 5`). The program prints the fastest route and the estimated travel time. |

Example route query on `example.cfe` (the travel time depends on where the buses happen to be):

```text
y
Откуда и куда Вы хотите поехать? 0 5
Самый быстрый маршрут:
Station1->Station2->Station5->Station6
Время в пути: ...
```

The prompts are in Russian: "Where do you want to go from and to?", "Fastest route:" and "Travel time:".

## Config format (`.cfe`)

A config file has three sections and ends with `END`. Each section header sits on its own line.

```text
STATIONLIST
#0 name="Station1" aboard="3"
#1 name="Station2" aboard="12"
...
ROUTELIST
#0 from="0" to="1" time="10"
#0 from="1" to="2" time="11"
#1 from="3" to="4" time="12"
#2 from="1" to="4" time="0"
BUSLIST
#0 amount="1"
#1 amount="2"
END
```

### `STATIONLIST`

One line per station. List them in order: IDs start at 0 and go up by 1.

| Field | Meaning |
| --- | --- |
| `#N` | Station ID |
| `name` | Display name (no spaces) |
| `aboard` | How many ticks a bus waits at this stop |

### `ROUTELIST`

One line per road segment.

| Field | Meaning |
| --- | --- |
| `#N` | ID of the line (lane) the segment belongs to |
| `from`, `to` | Station IDs at each end. Segments work in both directions. |
| `time` | Travel time in ticks |

List a line's segments in order (`0→1`, then `1→2`, …). The line is built by chaining them, and buses bounce back and forth between its two ends.

A segment with `time="0"` is a **transfer**: a walking link between stations on two different lines. In the example, `#2 from="1" to="4" time="0"` lets passengers change between line 0 at `Station2` and line 1 at `Station5`.

### `BUSLIST`

| Field | Meaning |
| --- | --- |
| `#N` | Line ID |
| `amount` | Number of buses on that line |

Each bus starts at a random stop on its line, heading in a random direction.

## How it works

| File | Role |
| --- | --- |
| `main.cpp` | Entry point and input loop |
| `classes/Classes.h` | Declarations of every struct and class |
| `classes/CFEReader.cpp` | Parses the `.cfe` file and wires everything together |
| `classes/Storage.cpp` | Shared state: adjacency matrix, stations, lines, buses, route cache |
| `classes/RouteManager.cpp` | Runs Dijkstra from every station at start-up; builds and caches routes |
| `classes/BusManager.cpp` | Bus movement, finding the closest bus, travel-time estimates |
| `classes/Path.cpp` | Linked-list route representation and pretty-printing |
| `classes/ManagerInstances.cpp` | Splits a route at transfers and sums up the time for each leg |

Under the hood:

1. **Routing.** Stations form a weighted graph. Dijkstra runs once per station at start-up, and edge weights include the stop's `aboard` time, so long stops are penalised. Results are stored in an all-pairs table, and paths are cached on first request.
2. **Simulation.** Each bus is either standing at a stop (counting down `aboard`) or driving (counting down `distance`). At the end of a line it turns around.
3. **Travel time.** For each leg of the route, the program fast-forwards a copy of every bus to the moment you reach the boarding stop and picks the one that arrives soonest. It then adds the wait and the ride time.

## Known limitations

This is a learning project, so some rough edges remain:

- The config path is hard-coded, and there's no way to quit except `Ctrl+C`.
- Station names can't contain spaces.
- Each station is assumed to belong to a single line. Transfers connect separate stations.
- Input isn't validated, so unknown station IDs or a malformed config can crash the program.
- The committed `main` binary is a Linux x86-64 build. Rebuild it on other platforms.
