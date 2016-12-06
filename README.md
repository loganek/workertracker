# WorkerTracker
WorkerTracker is a set of tools for monitoring and analyzing user activity.

## How does it work?
Background process logs informations about the currently used application with a timestamp. Another application (WorkerTracker Analyzer) provides analyzing functionality, by displaying and visualising amount of time that user spend in particular application.

<img src="http://serwer1661916.home.pl/img/wtanalyzer.png" width=300/> <img src="http://serwer1661916.home.pl/img/wtanalyzer-graph.png" width=300 />

## Installation
### Windows:
Download the [windows installer](https://github.com/loganek/workertracker/releases).

### Linux
Requirements:
 * Qt5Widgets
 * Qt5Charts
 * Boost 1.61

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

## Licensing
This project is developed under BeerWare license. See [LICENSE](https://github.com/loganek/workertracker/blob/master/LICENSE) file for more details.
