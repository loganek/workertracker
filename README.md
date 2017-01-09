# WorkerTracker
WorkerTracker is a set of tools for monitoring and analyzing your computer activity.

## How to run the app?
The project consists of two applications: 
 * workertracker - background process that tracks your activity and saves the data to a database. It's highly recommended to add the program to your autostart script. If you are Windows user, installer can do it automatically for you.
 * workertracker-analyzer - application for analyzing the data. It doesn't have to be run all the time, but only when you want to look at your data.

<img src="http://serwer1661916.home.pl/img/wtanalyzer.png" width=260/> <img src="http://serwer1661916.home.pl/img/wtanalyzer-graph.png" width=260 /> <img src="http://serwer1661916.home.pl/img/wtanalyzer-dayofweek.png" width=260 />

## Installation
### Windows:
Download the [windows installer](https://github.com/loganek/workertracker/releases).

### Linux
Download the [linux installer](https://github.com/loganek/workertracker/releases).

Requirements:
 * Qt5Widgets
 * Qt5Charts
 * Boost 1.61

## Do I need this?
If you:
 * want to analyze your daily computer activity and use the data for improving the productivity,
 * are curious how much time do you waste on entertainment sites,
 * are obsessed with profiling your life,

then the answer is YES!.

## Build
1. Download the sources:

  a) latest git-master version:
   ```shell
   $ git clone git@github.com:loganek/workertracker.git
   $ cd workertracker
   ```
  b) latest release: [sources](https://github.com/loganek/workertracker/releases) 

2. Compile the project
```shell
$ mkdir build
$ cd build
$ cmake ..
$ make
```
## Contributing

Please fork the parent repository and contribute back using [pull requests](https://github.com/loganek/workertracker/pulls). Features can be requested using [issues](https://github.com/loganek/workertracker/issues). All code, comments, and critiques are greatly appreciated.

## Licensing
This project is developed under BeerWare license. See [LICENSE](https://github.com/loganek/workertracker/blob/master/LICENSE) file for more details.
