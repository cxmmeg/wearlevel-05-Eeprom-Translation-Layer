# using BR-Tree time (TestHotPageToColdPage(1500)) : 4 mins

# bitmap without pagetable's cache TestHotPageToColdPage(1500) takes time: 9 mins

start time : 
20/12/2 15:33:56 

end time : 
20/12/2 15:43:6 

**next step:using heap-cache**

# bitmap with pagetable's cache TestHotPageToColdPage(1500) takes time: 10 mins

start time : 
20/12/4 15:43:30 

end time : 
20/12/4 15:53:6 

# bit map with pagetable's cache , dualpool border cache , TestHotPageToColdPage(1500) takes time: 6 mins

start time : 
20/12/8 15:42:28 

end time : 
20/12/8 15:48:42 

**TestHotPageToColdPage(1500) -> single page**

takes 3mins

[INFO main.cc:66] start start_time : 
[INFO rtc.cc:1501] 20/12/9 15:45:27 

[INFO main.cc:88] end time : 
[INFO rtc.cc:1501] 20/12/9 15:48:27 


**rm 5 borders**

takes 3mins

[INFO main.cc:66] start start_time : 
[INFO rtc.cc:1501] 20/12/9 20:49:37 

[INFO main.cc:88] end time : 
[INFO rtc.cc:1501] 20/12/9 20:52:33 


**++++++++++++SampleSimulation(200)++++++++++++++**


[INFO performance.cc:51] ++++++++++++ performance +++++++++++++++

[INFO performance.cc:52] write speed		105 B/sec
[INFO performance.cc:53] overhead ratio		1.064643
[INFO performance.cc:54] standard deviation	153.707520
[INFO performance.cc:55] RAM cost		572 B
[INFO performance.cc:56] total write cycles : 64490 
[INFO performance.cc:57] ------------ performance ---------------

++++++++++++++++Erase Cycle Pool Info+++++++++++++

hot pool : 
ppn : 13 	 erase cycle : 81 	 effective erase cycle : 15 
ppn : 15 	 erase cycle : 80 	 effective erase cycle : 14 
ppn : 21 	 erase cycle : 84 	 effective erase cycle : 18 
ppn : 23 	 erase cycle : 81 	 effective erase cycle : 15 
cold pool : 
ppn : 0 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 1 	 erase cycle : 97 	 effective erase cycle : 0 
ppn : 2 	 erase cycle : 97 	 effective erase cycle : 0 
ppn : 3 	 erase cycle : 97 	 effective erase cycle : 0 
ppn : 4 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 5 	 erase cycle : 97 	 effective erase cycle : 0 
ppn : 6 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 7 	 erase cycle : 97 	 effective erase cycle : 0 
ppn : 8 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 9 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 10 	 erase cycle : 97 	 effective erase cycle : 0 
ppn : 11 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 12 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 14 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 16 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 17 	 erase cycle : 97 	 effective erase cycle : 0 
ppn : 18 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 19 	 erase cycle : 97 	 effective erase cycle : 0 
ppn : 20 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 22 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 24 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 25 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 26 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 27 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 28 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 29 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 30 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 31 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 32 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 33 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 34 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 35 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 36 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 37 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 38 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 39 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 40 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 41 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 42 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 43 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 44 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 45 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 46 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 47 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 48 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 49 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 50 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 51 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 52 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 53 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 54 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 55 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 56 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 57 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 58 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 59 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 60 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 61 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 62 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 63 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 64 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 65 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 66 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 67 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 68 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 69 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 70 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 71 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 72 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 73 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 74 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 75 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 76 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 77 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 78 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 79 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 80 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 81 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 82 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 83 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 84 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 85 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 86 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 87 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 88 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 89 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 90 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 91 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 92 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 93 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 94 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 95 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 96 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 97 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 98 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 99 	 erase cycle : 66 	 effective erase cycle : 0 
ppn : 100 	 erase cycle : 85 	 effective erase cycle : 4 
ppn : 101 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 102 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 103 	 erase cycle : 67 	 effective erase cycle : 0 
ppn : 104 	 erase cycle : 97 	 effective erase cycle : 0 
ppn : 105 	 erase cycle : 95 	 effective erase cycle : 0 
ppn : 106 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 107 	 erase cycle : 97 	 effective erase cycle : 0 
ppn : 108 	 erase cycle : 96 	 effective erase cycle : 0 
ppn : 109 	 erase cycle : 96 	 effective erase cycle : 0 
ppn : 110 	 erase cycle : 65 	 effective erase cycle : 0 
ppn : 111 	 erase cycle : 96 	 effective erase cycle : 0 
ppn : 112 	 erase cycle : 65 	 effective erase cycle : 0 
---------------Erase Cycle Pool Info---------------

thresh_hold : 30 ,hotpool size : 4 , coldpool size : 109 


**----------------SampleSimulation(200)------------------**


**++++++++++++++++++++++++MultiWriteTest(120)+++++++++++++++++++++++++**

const unsigned long long ROM_SIZE	 = 2KB
const unsigned char	 LOGIC_PAGE_SIZE = 10;
const unsigned int	 THRESH_HOLD	 = 30;

[INFO performance.cc:51] ++++++++++++ performance +++++++++++++++
[INFO performance.cc:52] write speed        110 B/sec
[INFO performance.cc:53] overhead ratio     1.065034
[INFO performance.cc:54] standard deviation 99.433400
[INFO performance.cc:55] RAM cost       584 B
[INFO etl.cc:443] dualpool ram cost : 440 
[INFO etl.cc:445] pagetable ram cost : 132 
[INFO performance.cc:56] total write cycles : 46896 
[INFO performance.cc:57] ------------ performance ---------------

++++++++++++++++Erase Cycle Pool Info+++++++++++++

hot pool : 
ppn : 77     erase cycle : 61    effective erase cycle : 27 
ppn : 79     erase cycle : 60    effective erase cycle : 26 
ppn : 85     erase cycle : 51    effective erase cycle : 17 
ppn : 87     erase cycle : 49    effective erase cycle : 15 
cold pool : 
ppn : 0      erase cycle : 63    effective erase cycle : 0 
ppn : 1      erase cycle : 64    effective erase cycle : 0 
ppn : 2      erase cycle : 64    effective erase cycle : 0 
ppn : 3      erase cycle : 65    effective erase cycle : 0 
ppn : 4      erase cycle : 63    effective erase cycle : 0 
ppn : 5      erase cycle : 34    effective erase cycle : 0 
ppn : 6      erase cycle : 63    effective erase cycle : 0 
ppn : 7      erase cycle : 65    effective erase cycle : 0 
ppn : 8      erase cycle : 63    effective erase cycle : 0 
ppn : 9      erase cycle : 65    effective erase cycle : 0 
ppn : 10     erase cycle : 65    effective erase cycle : 0 
ppn : 11     erase cycle : 35    effective erase cycle : 0 
ppn : 12     erase cycle : 63    effective erase cycle : 0 
ppn : 13     erase cycle : 65    effective erase cycle : 0 
ppn : 14     erase cycle : 63    effective erase cycle : 0 
ppn : 15     erase cycle : 65    effective erase cycle : 0 
ppn : 16     erase cycle : 63    effective erase cycle : 0 
ppn : 17     erase cycle : 65    effective erase cycle : 0 
ppn : 18     erase cycle : 63    effective erase cycle : 0 
ppn : 19     erase cycle : 65    effective erase cycle : 0 
ppn : 20     erase cycle : 63    effective erase cycle : 0 
ppn : 21     erase cycle : 65    effective erase cycle : 0 
ppn : 22     erase cycle : 63    effective erase cycle : 0 
ppn : 23     erase cycle : 65    effective erase cycle : 0 
ppn : 24     erase cycle : 63    effective erase cycle : 0 
ppn : 25     erase cycle : 65    effective erase cycle : 0 
ppn : 26     erase cycle : 63    effective erase cycle : 0 
ppn : 27     erase cycle : 65    effective erase cycle : 0 
ppn : 28     erase cycle : 63    effective erase cycle : 0 
ppn : 29     erase cycle : 65    effective erase cycle : 0 
ppn : 30     erase cycle : 63    effective erase cycle : 0 
ppn : 31     erase cycle : 65    effective erase cycle : 0 
ppn : 32     erase cycle : 63    effective erase cycle : 0 
ppn : 33     erase cycle : 65    effective erase cycle : 0 
ppn : 34     erase cycle : 63    effective erase cycle : 0 
ppn : 35     erase cycle : 65    effective erase cycle : 0 
ppn : 36     erase cycle : 63    effective erase cycle : 0 
ppn : 37     erase cycle : 65    effective erase cycle : 0 
ppn : 38     erase cycle : 63    effective erase cycle : 0 
ppn : 39     erase cycle : 65    effective erase cycle : 0 
ppn : 40     erase cycle : 63    effective erase cycle : 0 
ppn : 41     erase cycle : 65    effective erase cycle : 0 
ppn : 42     erase cycle : 63    effective erase cycle : 0 
ppn : 43     erase cycle : 65    effective erase cycle : 0 
ppn : 44     erase cycle : 63    effective erase cycle : 0 
ppn : 45     erase cycle : 65    effective erase cycle : 0 
ppn : 46     erase cycle : 63    effective erase cycle : 0 
ppn : 47     erase cycle : 65    effective erase cycle : 0 
ppn : 48     erase cycle : 63    effective erase cycle : 0 
ppn : 49     erase cycle : 65    effective erase cycle : 0 
ppn : 50     erase cycle : 63    effective erase cycle : 0 
ppn : 51     erase cycle : 65    effective erase cycle : 0 
ppn : 52     erase cycle : 63    effective erase cycle : 0 
ppn : 53     erase cycle : 65    effective erase cycle : 0 
ppn : 54     erase cycle : 63    effective erase cycle : 0 
ppn : 55     erase cycle : 65    effective erase cycle : 0 
ppn : 56     erase cycle : 63    effective erase cycle : 0 
ppn : 57     erase cycle : 65    effective erase cycle : 0 
ppn : 58     erase cycle : 63    effective erase cycle : 0 
ppn : 59     erase cycle : 65    effective erase cycle : 0 
ppn : 60     erase cycle : 63    effective erase cycle : 0 
ppn : 61     erase cycle : 65    effective erase cycle : 0 
ppn : 62     erase cycle : 63    effective erase cycle : 0 
ppn : 63     erase cycle : 65    effective erase cycle : 0 
ppn : 64     erase cycle : 63    effective erase cycle : 0 
ppn : 65     erase cycle : 65    effective erase cycle : 0 
ppn : 66     erase cycle : 63    effective erase cycle : 0 
ppn : 67     erase cycle : 65    effective erase cycle : 0 
ppn : 68     erase cycle : 63    effective erase cycle : 0 
ppn : 69     erase cycle : 65    effective erase cycle : 0 
ppn : 70     erase cycle : 63    effective erase cycle : 0 
ppn : 71     erase cycle : 65    effective erase cycle : 0 
ppn : 72     erase cycle : 63    effective erase cycle : 0 
ppn : 73     erase cycle : 65    effective erase cycle : 0 
ppn : 74     erase cycle : 63    effective erase cycle : 0 
ppn : 75     erase cycle : 65    effective erase cycle : 0 
ppn : 76     erase cycle : 63    effective erase cycle : 0 
ppn : 78     erase cycle : 63    effective erase cycle : 0 
ppn : 80     erase cycle : 63    effective erase cycle : 0 
ppn : 81     erase cycle : 65    effective erase cycle : 0 
ppn : 82     erase cycle : 63    effective erase cycle : 0 
ppn : 83     erase cycle : 65    effective erase cycle : 0 
ppn : 84     erase cycle : 63    effective erase cycle : 0 
ppn : 86     erase cycle : 63    effective erase cycle : 0 
ppn : 88     erase cycle : 63    effective erase cycle : 0 
ppn : 89     erase cycle : 33    effective erase cycle : 0 
ppn : 90     erase cycle : 63    effective erase cycle : 0 
ppn : 91     erase cycle : 33    effective erase cycle : 0 
ppn : 92     erase cycle : 63    effective erase cycle : 0 
ppn : 93     erase cycle : 33    effective erase cycle : 0 
ppn : 94     erase cycle : 63    effective erase cycle : 0 
ppn : 95     erase cycle : 33    effective erase cycle : 0 
ppn : 96     erase cycle : 63    effective erase cycle : 0 
ppn : 97     erase cycle : 33    effective erase cycle : 0 
ppn : 98     erase cycle : 63    effective erase cycle : 0 
ppn : 99     erase cycle : 33    effective erase cycle : 0 
ppn : 100    erase cycle : 33    effective erase cycle : 0 
ppn : 101    erase cycle : 33    effective erase cycle : 0 
ppn : 102    erase cycle : 63    effective erase cycle : 0 
ppn : 103    erase cycle : 33    effective erase cycle : 0 
ppn : 104    erase cycle : 65    effective erase cycle : 0 
ppn : 105    erase cycle : 44    effective erase cycle : 8 
ppn : 106    erase cycle : 65    effective erase cycle : 0 
ppn : 107    erase cycle : 63    effective erase cycle : 0 
ppn : 108    erase cycle : 63    effective erase cycle : 0 
ppn : 109    erase cycle : 64    effective erase cycle : 0 
ppn : 110    erase cycle : 65    effective erase cycle : 0 
ppn : 111    erase cycle : 63    effective erase cycle : 0 
ppn : 112    erase cycle : 65    effective erase cycle : 0 
---------------Erase Cycle Pool Info---------------

thresh_hold : 30 ,hotpool size : 4 , coldpool size : 109 

[ 63,  63,  64,  62,  33,  62,  64,  62,  64,  64 ],
[ 34,  62,  64,  62,  64,  62,  64,  62,  64,  62 ],
[ 64,  62,  64,  62,  64,  62,  64,  62,  64,  62 ],
[ 64,  62,  64,  62,  64,  62,  64,  62,  64,  62 ],
[ 64,  62,  64,  62,  64,  62,  64,  62,  64,  62 ],
[ 64,  62,  64,  62,  64,  62,  64,  62,  64,  62 ],
[ 64,  62,  64,  62,  64,  62,  64,  62,  64,  62 ],
[ 64,  62,  64,  62,  64,  62,  60,  62,  59,  62 ],
[ 64,  62,  64,  62,  50,  62,  48,  62,  32,  62 ],
[ 32,  62,  32,  62,  32,  62,  32,  62,  32,  32 ],
[ 32,  62,  32,  64,  43,  64,  62,  62,  63,  64 ],


**----------------------MultiWriteTest(120)-------------------------**


**+++++++++++++++++relation btw write cycles and performance+++++++++++++++++**


[INFO performance.cc:51] ++++++++++++ performance +++++++++++++++

ROM_SIZE	 = 32KB
LOGIC_PAGE_SIZE = 10;
THRESH_HOLD	 = 30;

[INFO performance.cc:52] write speed        72 B/sec
[INFO performance.cc:53] overhead ratio     1.066078
[INFO performance.cc:54] standard deviation 530.181100
[INFO performance.cc:55] RAM cost       2020 B
[INFO performance.cc:56] total write cycles : 10079 

[INFO performance.cc:57] ------------ performance ---------------
**-----------------relation btw write cycles and performance-----------------**