# GOMOKU_

< SMART BOTS FOR A SIMPLE GAME />

## GOMOKU

**Binary name:** `pbrain-gomoku-ai`
**Language:** Anything as long as it works on "the dump"
**Compilation:** When necessary, via Makefile, including `re`, `clean`, and `fclean` rules

---

* The totality of your source files, except all useless files (binary, temp files, obj files, ...), must be included in your delivery.
* All the bonus files should be in a directory named `bonus`.

The goal of this project is to implement a Gomoku Narabe game bot (also called Wuzi Qi, Slope, Darpion or Five in a Row), focusing on the performance of its artificial intelligence.

Your bot must be compliant with the communication protocol (Epitech Mirror), at least the **Mandatory commands** part.

To run your bot you will need to use a game manager, here we will use **liskvork** which is entirely Free and Open Source, and has a wide variety of supported platforms/architecture.
`liskvork` is freely available on the internet as binaries or directly as source code (Epitech Mirror).

To get you started some very basic AI templates are available here for C++, Zig, Rust, Python...

Feel free to implement an algorithm of your choice for your bot (Min-max, Monte-Carlo, Machine Learning or other). You will be evaluated on the **efficiency** of your bot, and on **this criteria alone**.

You may need to develop a rules management algorithm; do not hesitate to enrich your board representation and your data structures to optimize this algorithm!

### Technical constraints

* Whatever development language you choose, your program must compile on Linux using your Makefile (if compilation is needed).
* Only standard libraries are allowed. No tensorflow or scikit-learn.

---

## Game Rules

This is a 2-player game that is played on a **20x20 game board** (called goban). We use the **freestyle ruleset** for this project.

Every player plays a stone on their turn, and the game ends as soon as one has **5 stones in a row** (vertically, horizontally or diagonally) — and thus wins.

---

## Evaluation

Your bot will be evaluated based on its results in actual game playing, via a 2-step process:

### 1. Play to win

Your program is intended to understand some winning situations and play the right move. We will send you pre-filled board states and your program must make the correct move. For example: if there is a winning move, your program must play it.

### 2. Outsmart local bots

Programs are matched against bots of low to medium levels. The aim is to **maximize your number of victories**.

---

## Brawl

This project also includes a tournament that is completely optional and opt-in named **Brawl**.
It does **not** impact your grade if you play or not.

Access URL: [https://gomoku.epitest.eu](https://gomoku.epitest.eu)

### How to opt-in

Create a file named `.brawl` at the root of your repository (it is a hidden file).

In it you can put the name of your AI, which will be displayed on the tournament's website.

* The name must be **5 to 16 characters long**.
* It may contain **alpha-numeric characters, spaces, dashes, and underscores**.
* Validation regex:

```
^[a-zA-Z _-]{5,16}$
```

If you need to compile, make sure that your make's `clean` rule does **not delete your binary**!
If it does, your AI will be marked as not building in the tournament.

### Tournament features

* Global national leaderboard
* Elo system (standard bayesian Elo system)
* Complete game list
* Elo over time analysis per brain with game stats (such as the win ratio)
* Replay system (you can watch any match played on the tournament, or locally)
* The solution to every single problem in the world (maybe)

Brawl has a **public API**.

---

## Technical constraints

During the whole process, the rules are as follows:

* **5 seconds maximum per move**
* **70 MB of memory per bot**
* A **forbidden move** automatically leads to defeat
* Use of a **forbidden library** leads to elimination

These rules apply to both the tournament and the automated tests.
