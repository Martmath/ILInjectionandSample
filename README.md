# ILInjectionandSample
Here combined three cool and really great projects:

http://www.codeproject.com/Articles/14058/Parsing-the-IL-of-a-Method-Body Sorin Serban

http://www.codeproject.com/Articles/26060/NET-Internals-and-Code-Injection  Daniel Pistelli

http://www.codeproject.com/Articles/463508/NET-CLR-Injection-Modify-IL-Code-during-Run-time Jerry.Wang

My small contribution are SetRealGit sample and variation on the theme of dynamic methods/AOP.

1)32bit generic method modify not work for net 4.5.3 

2)You should at least once to use native methods/types, which are used in universal methods(dynamic from TypeTemplate).  Differently tokens will be incorrectly defined.
For an example I used "new StringBuilder(s + IntPtr.Zero.ToString()+new IntPtr(100).ToString());" in "Magic" method special for this.

3)Magic may not work without two special strings:
RaiseIL.Dummy(RaiseIL.Dummy(), RaiseIL.Dummy(), RaiseIL.Dummy(), RaiseIL.Dummy(), RaiseIL.Dummy());
Trace.WriteLine(3.ToString());

4)Be Careful with casting structure to object in typetemplate sample.
This may not work.

5) 2015VS generated code different from the 2012 code. 
From what I found, and I know:
Ldloc_0 vs. dup (2015)
OpCodes.Ldobj vs. ldind.i  (for IntPtr/UintPtr in 2015) 

6)  2.0-3.5/64 net does not correctly handle the injection list created inside methods. 
I had to look for a workaround.

7)SetRealGit appeared only in the 4.6 framework, so not use it for 2.0-3.5 net injection.
