# Rock-Paper-Scissors Server

## Dependencies

None.


## Time Elepsed
See: Development Story


## Not Done Yet
* 
## Coding style

  [Google C++ style](http://google-styleguide.googlecode.com/svn/trunk/cppguide.html)


## References

* For IOCP: [A reusable, high performance, socket server class - Part 1](http://www.codeproject.com/Articles/2336/A-reusable-high-performance-socket-server-class-Pa)

* For Select: [Winsock 2 I/O Methods 5 Part 2](http://www.winsocketdotnetworkprogramming.com/winsock2programming/winsock2advancediomethod5a.html)


## Development Story

### First try for IOCP 7 hours, failed.

07:30 PM -- After reading some IOCP documents, start coding.
08:30 PM -- One hour passed, I am still writing (copying?) IOCP related code, God.
09:26 PM -- Two hours passed, still writing I~O~C~P related, hopeless.
			Why there isn't a book like <Mastering IOCP in 1 hour>?!
09:49 PM -- Back from 20 mins break.
10:46 PM -- Three hours passed, IOCPing, still. Dying.
11:12 PM -- Back from 13 mins watching "SPL 2015 R1 KT.Zest vs Samsung.Solar PvZ", Zest won.
11:57 PM -- The fourth hour passed, still writing IOCP related.
00:47 AM -- DEAD, I have run out of time. Now dead man walks.
01:47 AM -- almost one hour OVERDUE, it is raining outside. No chocolate, no latte.
			Oh, sorry, dead man should only care about fresh meat.
			I see the security coming, "Ahhh, Fresh MEAT!"
02:50 AM -- I admit that it is impossible for me to understand and write a good IOCP server tonight.
			Let me write the logic part, and change to old style multi-threads solution.
03:10 AM -- Go home, dead man sleeps.

### Second try for select 6 hours, done. But it is a player vs NPC game, is it right?

20:37 PM -- Back again, I will try `select` this time, which is reactor mode similiar to epoll and kqueue.
20:40 PM -- Found an prehistoric document about windows select, ugly coding style and buggy memory manangement. ref 2.
			Make it as a start point.
21:50 PM -- At last I clean the code up, now it is total different, new naming style, no longer an echo server,
			and management socket context with shared_ptrs instead of GlobeAlloc/Free.
23:40 PM -- Three hours passed. I think single thread is enough.
01:00 PM -- Sleepy.
01:50 PM -- I found that I have been writing a game which player fight with server(NPC) instead of a real world opponent.
			Oh no, I have to redesign the architecture, maybe later.
02:00 PM -- Client done.