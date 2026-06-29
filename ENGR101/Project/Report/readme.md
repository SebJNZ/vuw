# Usage
## Unit Testing Scripts
Ensure you have the following files in the path:
* `tester.c`
* `main.c`
* `main.h`
* `camera_mock.c`
* `camera_mock.h`

All of the tests are written under tester.c, under individual functions (`testLocateRuby()`, `testCalculateDetectionPoints()`, etc).
<br><br>
Compile:

`gcc tester.c camera_mock.c main.c -o tester`

Execute:

`./tester`

## Integration Testing
To ensure that my functions work together, I created an automated integration test. The test executes the main `start()` function, while a thread simulates the ruby being stolen after a 7 second delay.

Ensure you have the following files in the path:
* `integration.c`
* `main.c`
* `main.h`
* `camera_mock.c`
* `camera_mock.h`

Compile:

`gcc integration.c camera_mock.c main.c -o integration -lpthread`

Run:

`./integration`

To use multi-threading (for both `start()` and `stealRuby()` to run at the same time) I use `pthread.h` - this is a standard library on Linux, Unix, and MacOS, but not Windows.

# License
All code in this section is written by @SebJNZ, and is licensed under the GNU Affero General Public License v3.0. You must follow and understand all permissions, rules, and limitations stated under this license.
