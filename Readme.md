# Shannon–Fano kódoló és dekódoló
Made by [Péter Ferencz](https://peterferencz.me) \
Made for Budapest University of Technology and Economics | Faculty of Electrical Engineering and Informatics | BSc Computer Science and Engineering | Basics of Programming 1 (VIEEAA00)
![Logo of BME](./docs/res/BMEKicsi.png)

### Documentation ['HU']
[./docs/Documentation.pdf](./docs/Documentation.pdf)

### About the algorythm
[C. E. Shannon, „A Mathematical Theory of Communication”, 1948](https://web.archive.org/web/19980715013250/http://cm.bell-labs.com/cm/ms/what/shannonday/shannon1948.pdf) \
[Robert M. Fano, „The Transmittion of Information”, 1949](https://hcs64.com/files/fano-tr65-ocr-only.pdf)



### Building
Building is done with make specified in Makefile
Available build targets: `debug` (default), `release`
```sh
make build BUILD=[target]
```

### Usage
if `--bemenet` is not specified, stdin is selected \
if `--kimenet` is not specified, stdout is selected \
`--statisztika` and 'kodtabla' are optional flags \
`--help` is also implemented
```sh
bin [kodol|dekodol] --bemenet [input file] --kimenet [output file] --kodtabla --statisztika
```

### Licence
> Péter Ferencz, hereby disclaims all copyright interest in the program “Shannon–Fano kódoló és dekódoló” written by Péter Ferencz.

Licence file: [./Licence.txt](./Licence.txt)