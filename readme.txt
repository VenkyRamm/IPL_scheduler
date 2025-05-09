# IPL Tournament Simulator

## Overview
This is a C-based simulator that models a complete IPL-style cricket tournament. It schedules matches, simulates scores, calculates team standings, and optionally simulates playoffs. It includes weather-based rescheduling and allows queries for team-wise and venue-wise schedules.

## File Description
- `main.c` – The core logic for initialization, scheduling, simulation, and output.
- `playoffs.h` – Header file containing structures and logic for playoff simulation (must be present in the same directory).

## Compilation Instructions
To compile the program, use a C compiler like `gcc`:

```bash
gcc main.c -o ipl_sim
```

## Running the Program
Run the compiled binary:

```bash
./ipl_sim
```

## Program Flow
1. Initializes 8 IPL teams and their cities.
2. Generates a full round-robin schedule (56 matches).
3. Randomly simulates weather issues and reschedules if needed.
4. Displays the full match schedule and writes it to `schedule.txt`.
5. Offers a menu:
   - Option 1: Display matches for a specific team.
   - Option 2: Display matches at a specific venue.
   - Option 3: Exit menu and continue.
6. Simulates all matches with random scores.
7. Displays the final points table and asks if playoffs should be simulated.

## Outputs
- Console: Displays match schedules, team/venue-specific schedules, and final points table.
- File: `schedule.txt` contains the entire match schedule.

## Dependencies
- Standard C Libraries:
  - `stdio.h`
  - `stdlib.h`
  - `string.h`
  - `time.h`
- Note: Ensure `playoffs.h` is implemented and included properly for playoff simulation to work.

## Author
Developed by: ZETTABYTES
