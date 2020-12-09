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