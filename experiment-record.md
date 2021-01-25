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

RAM: 863(dualpool) + 1020B(pagetable)

ROM_SIZE	 = 16KB
LOGIC_PAGE_SIZE = 10;
THRESH_HOLD	 = 5;
write speed = 22B/sec

cycles : [ 5010, 10079, 15069, 20018, 25007, 30017, 35086, 40075, 45025, 50014, 55003, 60093, 65082, 70031, 75021, 80010, 85100, 90089, 95038, 100028, 105017, 110106, 115096, 120045, 125034, 130004, 135113, 140103, 145052, 150041, 155011, 160000, 165109, 170059, 175048, 180017, 185007, 190116, 195065, 200055 ]

overhead ratio : [ 1.2898, 1.3076, 1.3372, 1.3520, 1.3610, 1.3670, 1.3714, 1.3745, 1.3771, 1.3790, 1.3806, 1.3820, 1.3831, 1.3841, 1.3850, 1.3857, 1.3864, 1.3870, 1.3875, 1.3880, 1.3884, 1.3888, 1.3891, 1.3895, 1.3898, 1.3900, 1.3903, 1.3905, 1.3908, 1.3910, 1.3911, 1.3913, 1.3915, 1.3917, 1.3918, 1.3919, 1.3921, 1.3922, 1.3924, 1.3925  ]

standard deviation : [ 125.47, 76.54, 80.54, 83.76, 76.93, 80.49, 85.02, 75.99, 62.81, 46.03, 57.57, 74.28, 79.67, 83.06, 77.78, 84.66, 83.84, 76.84, 65.64, 49.84, 52.07, 71.54, 83.32, 82.29, 78.67, 81.71, 83.06, 77.54, 73.90, 53.89, 43.84, 70.51, 81.00, 81.60, 79.46, 79.20, 81.12, 78.34, 74.63, 56.75  ]
thresh_hold : 5 ,hotpool size : 5 , coldpool size : 904 

ROM_SIZE	 = 16KB
LOGIC_PAGE_SIZE = 10;
THRESH_HOLD	 = 30;
write speed = 66B/sec

cycles : [ 5010, 10079, 15069, 20018, 25007, 30017, 35086, 40075, 45025, 50014, 55003, 60093, 65082, 70031, 75021, 80010, 85100, 90089, 95038, 100028, 105017, 110106, 115096, 120045, 125034, 130004, 135113, 140103, 145052, 150041, 155011, 160000, 165109, 170059, 175048, 180017, 185007, 190116, 195065, 200055 ]

overhead ratio : [ 1.0659, 1.0661, 1.0630, 1.0557, 1.0512, 1.0483, 1.0461, 1.0445, 1.0463, 1.0482, 1.0498, 1.0512, 1.0524, 1.0534, 1.0543, 1.0551, 1.0556, 1.0562, 1.0568, 1.0572, 1.0577, 1.0581, 1.0584, 1.0588, 1.0591, 1.0594, 1.0596, 1.0598, 1.0600, 1.0603, 1.0605, 1.0606, 1.0608, 1.0610, 1.0611, 1.0613, 1.0614, 1.0616, 1.0616, 1.0618]

standard deviation : [372.99, 465.47, 525.04, 619.47, 660.74, 654.74, 600.15, 483.61, 439.92, 348.80, 118.30, 372.71, 445.54, 451.59, 400.79, 235.19, 302.54, 422.92, 451.81, 432.47, 331.34, 176.40, 382.99, 448.13, 449.28, 391.07, 196.25, 317.89, 427.80, 450.68, 428.34, 317.89, 209.41, 391.71, 448.95, 446.91, 381.95, 159.17, 334.19, 433.33  ]


ROM_SIZE	 = 16KB
LOGIC_PAGE_SIZE = 10;
THRESH_HOLD	 = 100;
write_speed : 115B/sec

cycles : [ 5010, 10079, 15069, 20018, 25007, 30017, 35086, 40075, 45025, 50014, 55003, 60093, 65082, 70031, 75021, 80010, 85100, 90089, 95038, 100028, 105017, 110106, 115096, 120045, 125034, 130004, 135113, 140103, 145052, 150041, 155011, 160000, 165109, 170059, 175048, 180017, 185007, 190116, 195065, 200055,  ]

overhead ratio : [ 1.0196, 1.0196, 1.0198, 1.0199, 1.0198, 1.0199, 1.0199, 1.0198, 1.0199, 1.0190, 1.0181, 1.0175, 1.0169, 1.0164, 1.0160, 1.0156, 1.0153, 1.0150, 1.0147, 1.0145, 1.0143, 1.0141, 1.0139, 1.0137, 1.0136, 1.0135, 1.0133, 1.0134, 1.0137, 1.0139, 1.0141, 1.0143, 1.0144, 1.0146, 1.0147, 1.0149, 1.0150, 1.0151, 1.0152, 1.0154,  ]

standard deviation : [ 694.01, 960.29, 1141.41, 1269.57, 1370.92, 1442.31, 1493.02, 1526.23, 1534.51, 1676.31, 1799.20, 1900.13, 1982.71, 2045.93, 2095.23, 2129.29, 2151.14, 2159.99, 2154.51, 2138.01, 2104.18, 2058.13, 1996.47, 1918.17, 1824.25, 1704.54, 1556.41, 1509.89, 1487.47, 1448.74, 1386.57, 1300.24, 1182.55, 1023.71, 804.06, 432.83, 552.81, 871.84, 1069.88, 1213.61,  ]

thresh_hold : 100 ,hotpool size : 4 , coldpool size : 905

**-----------------relation btw write cycles and performance-----------------**


**+++++++++++++++++dual cache : main_cache ratio with hit ratio++++++++++++++++++**
ROM_SIZE	 = 2KB
LOGIC_PAGE_SIZE = 10;
THRESH_HOLD	 = 100;
total write cycles : 10079 
dualcache hit rate 0.914700  
main cache rate : 0.7


ROM_SIZE	 = 2KB
LOGIC_PAGE_SIZE = 10;
THRESH_HOLD	 = 100;
total write cycles : 10079 
main cache rate : 0.6
dualcache hit rate 0.914700 

ROM_SIZE	 = 2KB
LOGIC_PAGE_SIZE = 10;
THRESH_HOLD	 = 100;
total write cycles : 10079 
main cache rate : 0.5
dualcache hit rate 0.914700 
 
ROM_SIZE	 = 2KB
LOGIC_PAGE_SIZE = 10;
THRESH_HOLD	 = 100;
total write cycles : 10079 
main cache rate : 0.1
dualcache hit rate 0.914700 
**----------------dual cache : main_cache ratio with hit ratio---------------------**


**+++++++++++++++++dual cache : dualcache size with hit ratio++++++++++++++++++**

ROM_SIZE	 = 2KB
LOGIC_PAGE_SIZE = 10;
cycle = 10000;

cache capacity ratio : 0.03
dualcache hit rate     0.667565
pagetable ram cost : 36B 

cache capacity ratio : 0.05
dualcache hit rate     0.667565
pagetable ram cost : 60B 

cache capacity ratio : 0.1
dualcache hit rate     0.667565
pagetable ram cost : 123B 

cache capacity ratio : 0.15
dualcache hit rate    0.66  
pagetable ram cost : 192B 

cache capacity ratio : 0.2
dualcache hit rate    0.980392  
pagetable ram cost : 264B 

 

**----------------dual cache : dualcache size with hit ratio---------------------**




**++++++++++++++++++++4.6+++++++++++++++**

负载自适应测试: 

T1,对同一个逻辑地址进行10,000次写入操作，每次写入长度20B的数据:

 { 224, 224, 222, 224, 222, 224, 222, 224, 222, 224, 222, 224, 222, 224, 222, 224, 222, 224, 222, 224, 222, 224, 222, 224, 222, 220, 222, 220, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 192, 222, 222, 224, 224, 222, 192, 222, 192 }
write speed            192 B/sec
spend time             1038 sec    =>  1120 sec (without pagetable cache)
overhead ratio         1.060300
standard deviation     144.180440
RAM cost               520 B
dualcache hit rate     0.999950        
total write cycles : 20000 

T2,随机地址写入

page erase cnt :{ 316, 267, 303, 285, 316, 333, 301, 295, 290, 305, 289, 272, 295, 302, 280, 288, 306, 312, 295, 288, 300, 287, 286, 287, 301, 301, 294, 308, 306, 288, 315, 299, 270, 281, 295, 279, 291, 288, 277, 289, 281, 259, 308, 284, 295, 285, 309, 299, 300, 277, 299, 294, 281, 307, 303, 303, 301, 318, 298, 294, 294, 307, 288, 294, 306, 310, 280, 309, 289, 310, 301, 288, 279, 296, 294, 289, 288, 306, 295, 289, 329, 294, 290, 303, 302, 299, 297, 301, 286, 287, 315, 307, 301, 288, 312, 296, 286, 281, 318, 287 }
write speed            204 B/sec
spend time             1086 sec    =>  1099 sec (without pagetable cache)
overhead ratio         1.035804
standard deviation     126.261630
RAM cost               520 B
dualcache hit rate     0.969135        
total write cycles : 28544 

缓存映射表测试: 

| 测试用例 | 请求数 | 页面总数 | 访问模式          |
| -------- | ------ | -------- | ----------------- |
| T1       | 10,000 | 1,000    | 随机访问          |
| T2       | 10,000 | 1,000    | 随机访问+顺序扫描 |

性能指标:  命中率  运行时间

**T1,2Q**
hitrate time    cachesize
0.33    53.00   40
0.45    47.00   80
0.58    39.00   120
0.66    34.00   160 

**T1,LRU**
0.37    51.00   40
0.52    42.00   80
0.62    37.00   120
0.69    32.00   160

**T1,no cache**
-       68      - 

**T2,no cache**
-       68      - 

**T2,2Q**
0.25    58.00   40
0.36    52.00   80
0.46    48.00   120
0.51    43.00   160

**T2,LRU**
0.23    60.00   40
0.31    57.00   80
0.36    54.00   120
0.43    50.00   160




**--------------------4.6---------------**


**+++++++++++++++PWL与DP初步对比+++++++++++++**
TestCase: MultiWriteSimulateTest(20000)

NOWL:
[INFO performance.cc:65] write speed            335 B/sec
[INFO performance.cc:66] spend time             896 sec
[INFO performance.cc:67] overhead ratio         1.000000
[INFO performance.cc:68] standard deviation     21610.196000
[INFO performance.cc:69] RAM cost               900 B
[INFO performance.cc:70] dualcache hit rate     0.999900        
[INFO performance.cc:71] total write cycles : 30033 
[INFO performance.cc:72] extra write cycles : 0 
{ 0, 113, 0, 0, 0, 0, 0, 0, 0, 0, 18700, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 11220, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
0, 0, 0, 0 }


PDP:
[INFO performance.cc:65] write speed            327 B/sec
[INFO performance.cc:66] spend time             918 sec
[INFO performance.cc:67] overhead ratio         1.009922
[INFO performance.cc:68] standard deviation     492.037600
[INFO performance.cc:69] RAM cost               420 B
[INFO performance.cc:70] dualcache hit rate     0.999900        
[INFO performance.cc:71] total write cycles : 30033 
[INFO performance.cc:72] extra write cycles : 298 
page ec:{ 377, 
387, 381, 249, 375, 246, 368, 247, 370, 245, 251, 243, 365, 243, 359, 241, 359, 240, 351, 239, 353, 
237, 347, 238, 341, 236, 343, 234, 337, 234, 333, 231, 333, 232, 325, 231, 327, 228, 323, 229, 317, 
227, 317, 225, 313, 225, 309, 224, 309, 223, 249, 222, 301, 220, 303, 220, 299, 219, 293, 249, 295, 
217, 291, 327, 287, 227, 285, 326, 279, 324, 281, 325, 277, 325, 273, 324, 273, 324, 269, 321, 267, 
322, 265, 316, 259, 323, 261, 317, 257, 322, 253, 319, 253, 319, 249, 318, 247, 319, 245, 317, 324, 
387, 243, 310, 325}

DP: TH=60
[INFO performance.cc:54] write speed            290 B/sec
[INFO performance.cc:55] overhead ratio         1.030866
[INFO performance.cc:56] standard deviation     266.634580
[INFO performance.cc:57] RAM cost               1260 B
[INFO performance.cc:58] total write cycles : 30033 
[INFO performance.cc:59] extra write cycles : 927 
page ec:{ 310, 
309, 300, 311, 310, 249, 249, 311, 310, 249, 309, 249, 310, 249, 310, 249, 310, 249, 310, 249, 310, 
249, 309, 249, 310, 249, 309, 249, 310, 249, 310, 249, 309, 249, 310, 249, 310, 249, 309, 249, 310, 
249, 309, 249, 309, 249, 310, 249, 309, 249, 249, 250, 310, 292, 310, 311, 309, 311, 310, 311, 309, 
311, 310, 311, 310, 311, 310, 311, 310, 311, 310, 311, 310, 311, 310, 311, 310, 311, 310, 311, 310, 
311, 310, 311, 310, 311, 310, 311, 310, 311, 310, 311, 310, 311, 310, 311, 310, 311, 310, 311, 310, 
311, 310, 311, 310, }

**---------------PWL与DP初步对比-------------**



**++++++++++++++PDP与DP详细对比++++++++++++++**
PDP:
endurace = 500

cycles : [ 552, 1034, 1516, 1998, 2490, 2982, 3474, 4015, 4497, 4979, 5461, 5953, 6495, 6976, 7458, 7940, 8432, 8924, 9476, 9958, 10439, 10921, 11403, 11955, 12437, 12919, 13400, 13882, 14374, 14876, 15418, 15900, 16382, 16863, 17355, 17897, 18379, 18861, 19343, 19835, 20326, 20818, 21360, 21842, 22324, 22806, 23298, 23839, 24321, 24803, 25285, 25767, 26269, 26761, 27302, 27784, 28266, 28748, 29230, 29711, 30203, 30745, 31227, 31709, 32191, 32672, 33154, 33636, 34118, 34610, 35102, 35594, 36075, 36617, 37099, 37581, 38063, 38544, 39026, 39508, 39990, 40472, 40953, 41435, 41917, 42399, 42881, 43362, 43844, 44326, 44748, 45219, 45701, 46173, 46645, 47126, 47608, 48020, 48502, 48923 ,49375, 49857 ]

overhead ratio : [ 1.0036, 1.0058, 1.0066, 1.0080, 1.0072, 1.0074, 1.0075, 1.0080, 1.0080, 1.0080, 1.0081, 1.0081, 1.0083, 1.0083, 1.0086, 1.0083, 1.0083, 1.0085, 1.0087, 1.0086, 1.0088, 1.0088, 1.0089, 1.0087, 1.0088, 1.0087, 1.0087, 1.0086, 1.0088, 1.0087, 1.0088, 1.0087, 1.0086, 1.0085, 1.0086, 1.0085, 1.0087, 1.0087, 1.0087, 1.0088, 1.0087, 1.0088, 1.0088, 1.0088, 1.0089, 1.0088, 1.0089, 1.0091, 1.0091, 1.0091, 1.0091, 1.0092, 1.0091, 1.0093, 1.0093, 1.0093, 1.0094, 1.0095, 1.0096, 1.0098, 1.0099, 1.0101, 1.0102, 1.0103, 1.0106, 1.0107, 1.0109, 1.0110, 1.0112, 1.0113, 1.0115, 1.0117, 1.0118, 1.0120, 1.0122, 1.0124, 1.0127, 1.0131, 1.0132, 1.0135, 1.0137, 1.0140, 1.0142, 1.0144, 1.0147, 1.0152, 1.0155, 1.0158, 1.0162, 1.0165, 1.0169, 1.0173, 1.0177, 1.0181, 1.0188, 1.0193, 1.0199, 1.0205, 1.0213, 1.0222,1.0230, 1.0242  ]

standard deviation : [ 332.10, 467.13, 578.67, 669.91, 729.12, 786.38, 843.92, 891.70, 936.66, 974.03, 997.55, 1025.95, 1052.94, 1075.32, 1098.13, 1108.33, 1122.93, 1128.67, 1139.29, 1139.39, 1144.81, 1140.41, 1140.66, 1150.95, 1194.33, 1224.23, 1248.95, 1257.44, 1283.58, 1296.16, 1313.76, 1325.97, 1338.25, 1338.64, 1335.84, 1342.83, 1330.90, 1332.64, 1316.06, 1307.51, 1294.53, 1274.09, 1259.16, 1232.98, 1206.97, 1185.92, 1147.07, 1103.57, 1066.49, 1011.77, 957.88, 905.29, 838.94, 752.36, 638.94, 551.57, 541.02, 530.97, 515.87, 503.05, 484.80, 462.06, 441.99, 420.71, 393.68, 365.20, 332.48, 294.79, 255.39, 220.16, 203.72, 208.33, 233.89, 252.62, 264.50, 271.76, 269.02, 265.38, 252.52, 235.82, 212.58, 177.52, 143.35, 120.69, 121.14, 144.82, 167.96, 187.85, 191.02, 181.72, 165.14, 125.57, 82.37, 69.35, 103.61, 121.11, 115.55, 93.08, 37.05, 65.00 ,82.19, 60.39 ]

write speed : [ 345, 333, 336, 333, 327, 327, 327, 329, 328, 327, 328, 327, 326, 327, 327, 328, 328, 328, 327, 328, 327, 327, 327, 328, 329, 329, 329, 329, 330, 329, 329, 329, 329, 330, 329, 329, 328, 328, 328, 328, 329, 328, 328, 328, 329, 329, 329, 328, 329, 329, 329, 329, 329, 329, 329, 329, 327, 327, 325, 324, 322, 321, 319, 319, 317, 316, 314, 313, 312, 310, 310, 310, 310, 309, 309, 309, 308, 308, 307, 307, 306, 306, 306, 306, 306, 306, 304, 302, 300, 300, 300, 300, 299, 299, 296, 293, 290, 287, 285, 284 ,284,284 ]

extra_ec:

DP:

TH = 20
cycles : [ 482, 964, 1376, 1847, 2319, 2801, 3283, 3694, 4176, 4658, 5059, 5531, 6013, 6435, 6906, 7388, 7790, 8261, 8743, 9165, 9636, 10118, 10530, 10991, 11473, 11895, 12367, 12848, 13260, 13722, 14203, 14625, 15097, 15579, 16000, 16452, 16934, 17415, 17827, 18309, 18731, 19182, 19664, 20146, 20557, 21039, 21461, 21922, 22394, 22876, 23287, 23769, 24191, 24653, 25124, 25606, 26018, 26499, 26981, 27383, 27854, 28336, 28748, 29230, 29711, 30113, 30585, 31066, 31488, 31960, 32442, 32853, 33315, 33797, 34218, 34690, 35172, 35583, 36045, 36527, 36949, 37420, 37902, 38324, 38775, 39257, 39679, 40150, 40632, 41054, 41505, 41987, 42469, 42881, 43362, 43784, 44246, 44717, 45199, 45611  ]
overhead ratio : [ 1.0913, 1.0954, 1.0908, 1.0872, 1.0850, 1.0821, 1.0819, 1.0836, 1.0855, 1.0861, 1.0876, 1.0884, 1.0893, 1.0894, 1.0899, 1.0906, 1.0913, 1.0916, 1.0916, 1.0920, 1.0923, 1.0926, 1.0926, 1.0929, 1.0932, 1.0932, 1.0936, 1.0935, 1.0936, 1.0938, 1.0940, 1.0942, 1.0941, 1.0943, 1.0943, 1.0945, 1.0944, 1.0946, 1.0947, 1.0949, 1.0949, 1.0948, 1.0949, 1.0951, 1.0952, 1.0951, 1.0951, 1.0952, 1.0952, 1.0953, 1.0952, 1.0953, 1.0954, 1.0955, 1.0956, 1.0955, 1.0955, 1.0956, 1.0957, 1.0956, 1.0957, 1.0957, 1.0958, 1.0958, 1.0958, 1.0959, 1.0959, 1.0960, 1.0960, 1.0959, 1.0960, 1.0960, 1.0960, 1.0960, 1.0960, 1.0961, 1.0961, 1.0961, 1.0961, 1.0961, 1.0962, 1.0962, 1.0962, 1.0962, 1.0962, 1.0963, 1.0963, 1.0963, 1.0963, 1.0963, 1.0963, 1.0964, 1.0963, 1.0964, 1.0963, 1.0964, 1.0963, 1.0964, 1.0964, 1.0964  ]
standard deviation : [ 94.47, 112.15, 137.22, 152.93, 153.56, 137.12, 107.76, 93.56, 25.98, 89.43, 106.40, 105.05, 77.10, 52.32, 98.40, 107.81, 100.67, 58.45, 80.11, 103.79, 106.22, 85.57, 20.17, 91.46, 108.07, 104.90, 73.63, 67.79, 99.45, 108.32, 95.60, 47.51, 82.46, 106.02, 106.52, 84.71, 46.84, 98.02, 108.98, 101.69, 66.84, 68.98, 102.72, 106.56, 92.53, 17.66, 87.45, 106.80, 104.70, 74.77, 56.51, 100.74, 107.30, 98.04, 52.10, 80.89, 104.34, 106.40, 84.14, 24.82, 93.01, 108.09, 102.60, 68.64, 69.66, 100.86, 107.42, 92.96, 42.73, 84.80, 106.54, 106.57, 83.09, 52.47, 95.39, 107.28, 98.63, 64.59, 72.53, 103.79, 107.68, 90.79, 21.70, 88.08, 107.39, 104.15, 77.23, 58.85, 100.87, 107.99, 98.43, 49.45, 84.12, 106.17, 106.97, 86.02, 34.34, 94.96, 108.48, 102.24]
write speed : [ 267, 275, 275, 275, 279, 280, 280, 279, 280, 280, 281, 280, 280, 281, 281, 281, 281, 281, 282, 282, 281, 281, 282, 281, 281, 281, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282, 282  ]
extra_ec:

TH = 50
cycles : [ 492, 964, 1446, 1928, 2409, 2891, 3383, 3865, 4407, 4889, 5370, 5852, 6334, 6816, 7298, 7779, 8261, 8743, 9225, 9707, 10188, 10670, 11152, 11634, 12116, 12597, 13079, 13561, 14043, 14525, 15006, 15488, 15970, 16452, 16934, 17415, 17897, 18379, 18861, 19343, 19824, 20306, 20788, 21270, 21752, 22233, 22715, 23197, 23679, 24161, 24642, 25124, 25606, 26088, 26570, 27051, 27533, 28015, 28497, 28979, 29460, 29942, 30424, 30846, 31328, 31869, 32291, 32773, 33255, 33737, 34218, 34700, 35182, 35664, 36146, 36627, 37109, 37591, 38073, 38555, 39036, 39518, 39990, 40472, 40953, 41435, 41917, 42399, 42881, 43362, 43844, 44326, 44808, 45290, 45771, 46253, 46735, 47217, 47699, 48180, 48662, 49144 ]
overhead ratio : [ 1.0325, 1.0373, 1.0373, 1.0384, 1.0382, 1.0380, 1.0369, 1.0354, 1.0345, 1.0342, 1.0339, 1.0337, 1.0335, 1.0329, 1.0327, 1.0327, 1.0328, 1.0333, 1.0335, 1.0339, 1.0341, 1.0340, 1.0343, 1.0345, 1.0347, 1.0348, 1.0351, 1.0353, 1.0354, 1.0356, 1.0357, 1.0358, 1.0358, 1.0358, 1.0360, 1.0360, 1.0362, 1.0363, 1.0363, 1.0364, 1.0365, 1.0366, 1.0367, 1.0365, 1.0366, 1.0367, 1.0368, 1.0369, 1.0369, 1.0370, 1.0370, 1.0371, 1.0371, 1.0371, 1.0371, 1.0371, 1.0372, 1.0372, 1.0372, 1.0373, 1.0373, 1.0374, 1.0374, 1.0374, 1.0374, 1.0374, 1.0374, 1.0374, 1.0375, 1.0375, 1.0376, 1.0376, 1.0376, 1.0377, 1.0377, 1.0377, 1.0376, 1.0376, 1.0377, 1.0377, 1.0377, 1.0378, 1.0378, 1.0378, 1.0378, 1.0379, 1.0379, 1.0378, 1.0378, 1.0378, 1.0379, 1.0379, 1.0379, 1.0380, 1.0379, 1.0380, 1.0380, 1.0380, 1.0380, 1.0380, 1.0380, 1.0380]
standard deviation : [ 153.73, 204.96, 236.41, 254.87, 265.03, 284.57, 319.84, 345.24, 364.38, 370.56, 371.78, 367.36, 357.49, 338.45, 309.72, 270.35, 256.74, 246.04, 224.05, 186.62, 123.60, 83.92, 171.25, 214.09, 240.51, 255.71, 260.31, 258.14, 245.49, 222.27, 184.67, 116.51, 99.33, 174.52, 216.26, 242.36, 255.07, 259.62, 255.18, 242.60, 218.28, 176.36, 102.83, 106.88, 179.43, 219.27, 242.72, 256.44, 259.06, 254.98, 240.94, 214.51, 174.05, 91.84, 118.53, 184.89, 223.23, 247.28, 257.79, 260.63, 255.37, 240.76, 215.31, 177.08, 104.01, 125.18, 182.07, 220.91, 243.77, 256.61, 258.14, 254.31, 239.00, 211.84, 168.59, 81.09, 123.05, 186.44, 223.37, 246.19, 256.96, 259.62, 253.23, 238.51, 210.89, 165.08, 75.11, 128.55, 191.68, 225.83, 246.87, 257.48, 258.68, 252.85, 235.25, 205.96, 157.78, 47.93, 136.92, 193.01, 227.40, 248.26  ]
write speed : [ 289, 292, 289, 287, 290, 289, 289, 288, 289, 289, 290, 289, 289, 288, 289, 290, 289, 290, 290, 290, 291, 290, 291, 290, 291, 290, 291, 291, 291, 291, 291, 291, 291, 291, 291, 292, 291, 292, 291, 292, 292, 292, 291, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 292, 293, 292, 293, 292, 292, 293, 292, 293, 292, 293, 293, 293, 293, 292, 293, 293, 293  ]
extra_ec:


TH = 80
cycles : [ 492, 1034, 1516, 1998, 2480, 2961, 3443, 3925, 4407, 4889, 5381, 5872, 6364, 6906, 7388, 7870, 8352, 8833, 9315, 9797, 10289, 10831, 11313, 11794, 12276, 12758, 13240, 13722, 14203, 14695, 15187, 15669, 16161, 16643, 17185, 17666, 18148, 18630, 19112, 19594, 20075, 20557, 21039, 21531, 22013, 22555, 23036, 23518, 24000, 24482, 24964, 25445, 25927, 26409, 26901, 27383, 27875, 28367, 28848, 29390, 29872, 30354, 30836, 31317, 31799, 32281, 32763, 33245, 33737, 34218, 34760, 35242, 35724, 36206, 36687, 37169, 37651, 38133, 38615, 39096, 39588, 40080, 40562, 41054, 41536, 42078, 42559, 43041, 43523, 44005, 44487, 44968, 45450, 45932, 46424, 46966, 47448, 47929, 48411, 48893, 49375, 49857 ]
overhead ratio : [ 1.0203, 1.0232, 1.0224, 1.0240, 1.0242, 1.0243, 1.0238, 1.0245, 1.0236, 1.0231, 1.0232, 1.0228, 1.0225, 1.0220, 1.0218, 1.0216, 1.0211, 1.0213, 1.0211, 1.0210, 1.0209, 1.0207, 1.0206, 1.0205, 1.0207, 1.0205, 1.0206, 1.0206, 1.0209, 1.0210, 1.0211, 1.0213, 1.0213, 1.0215, 1.0214, 1.0215, 1.0215, 1.0215, 1.0217, 1.0218, 1.0219, 1.0218, 1.0220, 1.0221, 1.0221, 1.0222, 1.0223, 1.0223, 1.0223, 1.0224, 1.0225, 1.0224, 1.0223, 1.0225, 1.0225, 1.0225, 1.0226, 1.0226, 1.0226, 1.0227, 1.0227, 1.0228, 1.0227, 1.0228, 1.0229, 1.0229, 1.0229, 1.0230, 1.0229, 1.0229, 1.0229, 1.0230, 1.0230, 1.0230, 1.0230, 1.0231, 1.0231, 1.0231, 1.0231, 1.0231, 1.0232, 1.0232, 1.0232, 1.0232, 1.0233, 1.0232, 1.0232, 1.0232, 1.0232, 1.0232, 1.0233, 1.0232, 1.0233, 1.0233, 1.0233, 1.0234, 1.0234, 1.0233, 1.0234, 1.0234, 1.0234, 1.0234 ]
standard deviation : [ 189.89, 271.78, 321.07, 353.13, 378.19, 398.31, 411.18, 414.81, 431.08, 471.73, 505.97, 529.28, 550.02, 564.82, 577.47, 587.04, 593.55, 587.74, 578.64, 566.79, 551.75, 532.50, 509.41, 481.75, 442.26, 409.72, 407.05, 398.04, 379.81, 355.75, 326.06, 279.93, 212.53, 100.84, 162.60, 244.78, 300.26, 341.51, 368.06, 387.52, 402.20, 412.04, 412.25, 408.71, 401.83, 384.52, 361.61, 333.19, 292.55, 231.77, 134.67, 127.56, 227.23, 285.26, 328.29, 361.40, 383.30, 397.25, 407.55, 410.49, 407.81, 400.89, 388.63, 366.21, 336.64, 298.65, 247.06, 160.46, 71.45, 206.66, 279.18, 322.76, 356.90, 380.63, 396.07, 405.66, 411.71, 410.74, 404.08, 392.02, 375.16, 347.54, 309.80, 260.53, 193.13, 44.70, 198.37, 269.45, 316.06, 350.30, 377.05, 396.02, 407.19, 411.99, 413.83, 408.22, 396.71, 380.66, 355.94, 321.85, 275.27, 212.65  ]
extra_ec:

TH = 110
cycles : [ 552, 1034, 1516, 1998, 2480, 2961, 3443, 3935, 4427, 4919, 5461, 5943, 6424, 6906, 7388, 7880, 8422, 8904, 9385, 9867, 10349, 10831, 11333, 11825, 12367, 12848, 13330, 13812, 14294, 14776, 15328, 15809, 16291, 16773, 17255, 17737, 18229, 18720, 19212, 19754, 20236, 20718, 21200, 21681, 22163, 22655, 23197, 23679, 24161, 24642, 25124, 25606, 26098, 26580, 27072, 27564, 28105, 28587, 29069, 29551, 30033, 30514, 31006, 31548, 32030, 32512, 32994, 33475, 33957, 34439, 34941, 35423, 35915, 36457, 36938, 37420, 37902, 38384, 38866, 39358, 39899, 40381, 40863, 41345, 41827, 42308, 42790, 43292, 43774, 44266, 44808, 45290, 45771, 46253, 46735, 47217, 47709, 48251, 48732, 49214, 49696, 50178  ]
overhead ratio : [ 1.0145, 1.0155, 1.0172, 1.0170, 1.0177, 1.0169, 1.0174, 1.0178, 1.0176, 1.0175, 1.0176, 1.0175, 1.0171, 1.0172, 1.0169, 1.0162, 1.0163, 1.0161, 1.0162, 1.0160, 1.0158, 1.0157, 1.0155, 1.0154, 1.0154, 1.0153, 1.0155, 1.0151, 1.0150, 1.0152, 1.0150, 1.0151, 1.0150, 1.0148, 1.0149, 1.0149, 1.0150, 1.0150, 1.0151, 1.0152, 1.0153, 1.0154, 1.0153, 1.0155, 1.0156, 1.0156, 1.0156, 1.0156, 1.0155, 1.0156, 1.0157, 1.0157, 1.0157, 1.0158, 1.0158, 1.0159, 1.0160, 1.0160, 1.0160, 1.0161, 1.0160, 1.0161, 1.0162, 1.0162, 1.0162, 1.0163, 1.0162, 1.0163, 1.0163, 1.0163, 1.0162, 1.0163, 1.0163, 1.0163, 1.0164, 1.0164, 1.0164, 1.0164, 1.0164, 1.0164, 1.0165, 1.0165, 1.0165, 1.0166, 1.0165, 1.0166, 1.0166, 1.0166, 1.0166, 1.0166, 1.0167, 1.0167, 1.0167, 1.0167, 1.0166, 1.0167, 1.0167, 1.0167, 1.0167, 1.0167, 1.0167, 1.0168  ]
standard deviation : [ 236.39, 320.11, 381.79, 426.83, 466.06, 497.87, 517.99, 537.72, 555.70, 562.11, 567.84, 576.01, 617.94, 663.63, 691.61, 718.38, 738.48, 755.43, 774.36, 782.72, 798.22, 801.64, 799.59, 802.56, 793.54, 792.10, 784.59, 772.10, 758.65, 732.42, 710.11, 680.37, 650.76, 625.14, 567.64, 561.08, 557.37, 546.88, 531.22, 512.27, 484.79, 453.98, 420.18, 365.22, 300.49, 211.90, 101.26, 236.73, 321.98, 377.37, 426.30, 466.38, 492.88, 516.89, 538.77, 550.53, 560.68, 565.64, 563.40, 561.43, 557.60, 541.95, 525.50, 503.49, 474.43, 442.16, 400.06, 340.44, 269.20, 158.62, 152.75, 270.05, 349.82, 405.16, 447.84, 481.21, 504.49, 526.09, 544.67, 552.18, 560.33, 563.51, 560.49, 557.25, 547.95, 531.12, 514.11, 491.92, 457.07, 417.27, 363.92, 297.42, 204.86, 70.11, 216.75, 309.26, 377.85, 427.28, 463.14, 494.07, 515.45, 536.33  ]
write speed : [ 290, 295, 291, 293, 291, 293, 291, 291, 291, 292, 292, 291, 292, 291, 292, 291, 292, 291, 292, 292, 292, 292, 292, 292, 293, 292, 292, 293, 292, 292, 292, 292, 293, 293, 293, 293, 293, 293, 293, 293, 294, 294, 294, 294, 294, 294, 294, 294, 294, 294, 294, 294, 294, 295, 294, 295, 294, 295, 295, 295, 295, 295, 295, 295, 295, 295, 295, 295, 295, 295, 295, 295, 295, 295, 295, 295, 295, 295, 295, 295, 295, 295, 295, 295, 296, 296, 296, 296, 296, 296, 295, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296, 296 ]
extra_ec:

**--------------PDP与DP详细对比--------------**
