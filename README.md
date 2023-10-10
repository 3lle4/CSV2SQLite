
# CSV2SQLite: Building a database with SQLite using the UNESCO Intangible Cultural Heritage (ICH) dataset as an example.

## Table of Contents

- [Project Overview](##project-overview)
- [Installation](##installation)
- [Usage](##usage)
- [Acknowledgements](##acknowledgements)

## Project Overview

The UNESCO Intangible Cultural Heritage (ICH) dataset is a collection of valuable cultural heritage information in mostly text form. This project showcases how to create an SQLite database from CSV files, import data into the database, and perform basic SQL operations.

Key features of the project:

- Safe CSV data to a C struct
- Import the struct into SQLite
- Search and query the database

The dataset can be found on [kaggle.com](https://www.kaggle.com/datasets/joebeachcapital/unesco-dive-into-intangible-cultural-heritage).
  


## Installation

1. Clone the repository

```shell
  git clone https://github.com/3lle4/CSV2SQLite
```

2. Navigate to the folder

```shell
    cd src
```

3. Compile the code

```shell
    ./compile main.c
```

## Usage

Use the given code to create a database from a CSV file and explore it with the given functions. Currently the following features are implemented: 
- Creating a database 
- Creating a table (from CSV file)
- Formatting the data types of columns
- Browsing and output of values


## Acknowledgements
 - [C Tutorial on tutorialspoint.com](https://www.tutorialspoint.com/cprogramming/index.htm)
 - [SQLite Tutorial on tutorialspoint.com](https://www.tutorialspoint.com/sqlite/index.htm)
 - [SQLite Tutorial from zetcode.com](https://zetcode.com/db/sqlitec/)
 - [another SQLite Tutorial, algoLancer at YouTube](https://www.youtube.com/watch?v=-C5HSdPU3TI&t=1s)
