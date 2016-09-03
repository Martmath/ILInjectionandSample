using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Reflection;
using System.Text;
using System.Windows.Forms;

public static class AOP_Test
{    
    public static event Addition.UI ToLog;
    public static bool ToLogisNull() { return ToLog == null; }
    public static void b1(object Me, MethodBase M, List<object> O)
    {
        string s = "Begin injection method var.1 for type ="+((Me == null) ? "(static)" : Me.GetType().ToString()) + "." + M.Name;
        if (ToLog == null) MessageBox.Show(s);
        else  ToLog(s);
    }

    public static void e1(object R, object Me, MethodBase M, List<object> O)
    {
        string s = "End injection method var.1 for type =" + ((Me == null) ? "(static)" : Me.GetType().ToString()) + "." + M.Name;
        if (ToLog == null) MessageBox.Show(s);
        else ToLog(s);

    }

    public static void b2(object Me, MethodBase M, List<object> O)
    {
        string s = "Begin injection method VAR.2 for type =" + ((Me == null) ? "(static)" : Me.GetType().ToString()) + "." + M.Name;
        if (ToLog == null) MessageBox.Show(s);
        else ToLog(s);
    }

    public static void e2(object R, object Me, MethodBase M, List<object> O)
    {
        string s = "End injection method VAR.2 for type =" + ((Me == null) ? "(static)" : Me.GetType().ToString()) + "." + M.Name;
        if (ToLog == null) MessageBox.Show(s);
        else ToLog(s);
    }
}

public struct testStruct
{
    public int int1;
    public static int int2;
}

public class testType
{  
    public object test1(object a0, object a1)//, long a2, IntPtr a3, ref Book a4, ref AOP a5)
    {        
        RaiseIL.Dummy(RaiseIL.Dummy(), RaiseIL.Dummy(), RaiseIL.Dummy(), RaiseIL.Dummy(), RaiseIL.Dummy());
        Trace.WriteLine(3.ToString());
        return a0.ToString() + a1;
    }

    public object test1_(object a0, object a1)//, long a2, IntPtr a3, ref Book a4, ref AOP a5)
    {
       return AOP.Do(this, MethodBase.GetCurrentMethod(), new MList() { a0, a1 });
    }

    public static object test1_s(object a0, object a1)//, long a2, IntPtr a3, ref Book a4, ref AOP a5)
    {
        return AOP.SDo(MethodBase.GetCurrentMethod(), new MList() { a0, a1 });
    }

    public object test(object a0, ref char a1, long a2, UIntPtr a3, ref UIntPtr a4, testStruct a5, ref testStruct a6, ref AOP a7, DateTime a8, ref DateTime a9)
    {
        return Do(this, MethodBase.GetCurrentMethod(), new MList() { a0, a1, a2, a3, a4, a5, a6, a7, a8, a9 });
      /*0000 : ldarg.0  (2) 
        0001 : ldarg.0  (2) 
        0002 : call System.Reflection.MethodBase System.Reflection.MethodBase::GetCurrentMethod()  (40|145|0|0|10) 167772305
        0007 : newobj instance void MList::.ctor()  (115|88|0|0|6) 100663384
        0012 : stloc.0  (10) 
        0013 : ldloc.0  (6) 
        0014 : ldarg.1  (3) 
        0015 : callvirt instance System.Void MList::Add()  (111|91|0|0|6) 100663387
        0020 : ldloc.0  (6) 
        0021 : ldarg.2  (4) 
        0022 : callvirt instance System.Void MList::Add()  (111|91|0|0|6) 100663387
        0027 : ldloc.0  (6) 
        0028 : call instance System.Object testType::Do()  (40|96|0|0|6) 100663392
        0033 : ret  (42) */
    }

    public static object test9_s(object a0, object a1)//, long a2, IntPtr a3, ref Book a4, ref AOP a5)
    {     
        MList.M = MethodBase.GetCurrentMethod();
        MList.L.Add(a0);
        MList.L.Add(a1);
        return AOP.SDoS();
    }

    public static void test8_s(object a0, object a1)//, long a2, IntPtr a3, ref Book a4, ref AOP a5)
    {
        MList.M = MethodBase.GetCurrentMethod();
        MList.L.Add(a0);
        MList.L.Add(a1);
        AOP.SDoS();
    }

    public object test0()//object inputStringasObject0,ref System.Int64 inputStringasObject1)//, long a2, IntPtr a3, ref IntPtr a4, ref AOP a5, testStruct a6,ref testStruct a7)
    {
        MList.O = this;
        MList.M = MethodBase.GetCurrentMethod();
       // MList.L.Add(inputStringasObject0);  MList.L.Add(inputStringasObject1); 
        return AOP.DoS();  
       // return Do(this, MethodBase.GetCurrentMethod(), MList.L); 
    }

    public object Do(object Me, MethodBase M, List<object> O) { return 0;}

    public static int testOutsideFieldint = 0;  
    //=====================
    public static void statictest1(ref int a0, string a1, long a2, IntPtr a3)//, ref int a0, string a1, long a2, IntPtr a3, ref Book a4, ref AOP a5)
    {
        RaiseIL.Dummy(RaiseIL.Dummy(), RaiseIL.Dummy(), RaiseIL.Dummy(), RaiseIL.Dummy(), RaiseIL.Dummy());
        Trace.WriteLine(3.ToString());
        testOutsideFieldint = 444 +a0+ Convert.ToInt32(a1)+ (int)a2+ a3.ToInt32();     
    }
   
    char testOutsideFieldchar;
    //=====================
    public void Append(char r)
    {
        testOutsideFieldchar = r;
    }

    public class testType2
    {
        public int int1;
        public static int int2;
    }    

    public object test3(ref object inputString0, string inputString1)
    {
        RaiseIL.Dummy(RaiseIL.Dummy(), RaiseIL.Dummy(), RaiseIL.Dummy(), RaiseIL.Dummy(), RaiseIL.Dummy());
        Trace.WriteLine(3.ToString());
        var test1 = new testType2();
        test1.int1 = 99;
        testType2.int2 = 7;
        StringBuilder test2 = new StringBuilder();
        test2.Append('h');
        test2.Append(test1.int1);
        test2.Append(testType2.int2);
        IntPtr testIntptr = new IntPtr(88);
        testIntptr = IntPtr.Zero;
        int i = testIntptr.ToInt32();
        test2.Append('h');
        this.Append('h');
        MessageBox.Show(test2.Capacity.ToString());
        return inputString0.ToString() + inputString1 + test2.ToString() + i.ToString();
    }

    public object test4(object inputtestType)
    {
        RaiseIL.Dummy(RaiseIL.Dummy(), RaiseIL.Dummy(), RaiseIL.Dummy(), RaiseIL.Dummy(), RaiseIL.Dummy());
        Trace.WriteLine(3.ToString());
        StringBuilder stringBuildertest = (StringBuilder)inputtestType;
        stringBuildertest.Append('h');
        testType testTypetest = (testType)inputtestType;
        testTypetest.Append('h');
        MessageBox.Show(stringBuildertest.Capacity.ToString());
        return 7;
    }
}
