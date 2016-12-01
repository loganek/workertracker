# WorkerTracker
WorkerTracker is a set of tools for monitoring and analyzing user activity.

## How does it work?
Background process logs informations about the currently used application with a timestamp. Another application (WorkerTracker Analyzer) provides analyzing functionality, by displaying and visualising amount of time that user spend in particular application.

<img src="https://lh3.googleusercontent.com/TZdjY42sp1Vh7DMa7c8iJam44SkkTEcna7M2WaiHylOcbJZ2aKP73X9e-7MlRHdB_mTMsynTp3iRYD4xkdJZLfQTBp39C-BF--03VP_wjgpS-baSUUag-na9SKwE6NRU41ToSfQs1-bD_moF5R_UJWW8qRjoJfCj-mgU8eI1EG4p8KG63NPUUvjHF0GUx9nuLIuyTReIFgDKZLjGqw-seqb1x7OGmA2px4VSeRZcUsZ8lmspq1o1coJpdkmA7bqsiffcLAjIfimQKQCqBqf1YPw-DtfW8L4n5bXYCGj9BW27ZNwQlJeFR9jmc7gBsmvVNLLo6jJdIBlyu4MwSNuz_ncCEnhNQqOs45IKRtUO7fnLJR9Y9-fjxvE9J6rrReNyZyBHVhNrSBhPg8VfpvoinJ3nrifDAz9ohYJ5-z4LbZxdeyN1yqBTlzkbzpkEhUgqI-o5MsJeoDXrMdDmuhTXC6TeicyShLHsgWpzO_5tqEaaMEi5PnD4xBUrlKTAnmuhNNla210MFcYidU4AGW7GXZ6mhQFogP_eVUaQLVHk_dESHlUJrlDuihOy6Z2yhJMVWr25HVkMsKCmgA12oCTHP4TaAJljlk9PAbt4ruY_D86l5OpQlg=w810-h646-no" width=300/> <img src="https://lh3.googleusercontent.com/U-cejUbIzzC8Sq2q73m-P__e4FRG7yL1bgwNqrk0ue9IlmJnlyt0cJvh1kE8Yv6NJmMDMhoqmds3e9emkwOiiLLayWYERiziY7V25IE8Fq07q3Pqgl5rxeQ9gCewgTA9oLhfudTgJkYfHTjg-lYPSlpBp6W5K8Ln7TFueu2n-ptJK_j-tgf9zQcwcmmhXltJV19eR9RvIekdEY74XS5yuR2ytezyTJVOhyBTPi7btMDZJkMcdGed4yp4q9KNXSHO1UWYP2Fp1ThrejobgN_fqu66HUEQyBxZJvToyhfXa_sR-2vqi2IJMgVa0imBBgEsrkEI1aqyrKRp6Jkjiv_ldJILUQjb6R4ijZ2io_B9DNlEGRyZYcot3-2Gd4uf0cGkGtAu3w2JSd8c_7GTUHLE-h-tAEZm6lqbYGmB8w3pF7Q_Bg20wIQ4Gr1ztpk3LdThusJ_3Sw3DVv_COHZ8GmnSlHIfj--WqoysdUJItHgZdEdJg06bw-Ya02nsaeOcS9eHKaUJc5d9467gS0gey0CqcERQOq_ehqup3Nl-dJCfi6S7Epk1B71dZTClh4bKIVc56-UiOVz-NiJ7GOSuDWWhco1brdB2Bfh4QNolcmTD6v2OmhQwA=w800-h633-no" width=300 />

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
