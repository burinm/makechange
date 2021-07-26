# **makechange**
## Sample code for making change

### To compile:
```
git clone https://github.com/burinm/makechange.git
cd makechange
make clean; make
```

### To run with standard denominations:
```
./change
```

### To run with alternate denominations ($2 bill):
```
./change_alt
```

#### Example
```
$ ./change \$11.00
[Initalized with 11 denominations]
	(1) $100 dollar bill
	(2) $50 dollar bill
	(3) $20 dollar bill
	(4) $10 dollar bill
	(5) $5 dollar bill
	(6) $1 dollar bill
	(7) 50¢ coin
	(8) 25¢ coin
	(9) 10¢ coin
	(10) 5¢ coin
	(11) 1¢ coin

[Dollars in: 11] [Cents in: 0]
	Checksum passed: 1100 (checksum) == 1100 (in)

[Change]
	1 x $10 dollar bills
	1 x $1 dollar bills
```
#### Example 2
```
$ ./change 9999.99
[Initalized with 11 denominations]
	(1) $100 dollar bill
	(2) $50 dollar bill
	(3) $20 dollar bill
	(4) $10 dollar bill
	(5) $5 dollar bill
	(6) $1 dollar bill
	(7) 50¢ coin
	(8) 25¢ coin
	(9) 10¢ coin
	(10) 5¢ coin
	(11) 1¢ coin

[Dollars in: 9999] [Cents in: 99]
	Checksum passed: 999999 (checksum) == 999999 (in)

[Change]
	99 x $100 dollar bills
	1 x $50 dollar bills
	2 x $20 dollar bills
	1 x $5 dollar bills
	4 x $1 dollar bills
	1 x 50¢ coin
	1 x 25¢ coin
	2 x 10¢ coin
	4 x 1¢ coin
```
