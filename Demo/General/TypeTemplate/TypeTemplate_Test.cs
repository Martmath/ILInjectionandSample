using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Linq;
using System.Text;
using System.Windows.Forms;

public class TypeTemplate0
{
    public static IntPtr Zero;
    public string stringFieldTest = "0";
    public StringBuilder Append(char h) { MessageBox.Show("If u see it - u baad progger"); return new StringBuilder(); }
    public TypeTemplate0(string i) { MessageBox.Show("If u see it - u baad progger"); stringFieldTest = i; }
    public TypeTemplate0() { MessageBox.Show("If u see it - u baad progger"); }
}

public class TypeTemplate1
{
    public int i;
    public void Msg(string s) { MessageBox.Show("If u see it - u baad progger"); }
}

 public class TypeTemplate2//IntPtr - struct soo sometime have ploblem with cast
 {
     public static IntPtr Zero;
     public long ToInt64() { MessageBox.Show("If u see it - u baad progger"); return 111; }
 }

 public class instance_MyStringBuilder
 {
     public static event Addition.UI ToLog;
     public static bool ToLogisNull() { return ToLog == null; }
     public string stringFieldTest = "0";
     public instance_MyStringBuilder(string S)
     {
         stringFieldTest = S;
         string s = "constructor instance_MyStringBuilder with parameters " + S.ToString();
         if (ToLog == null) MessageBox.Show(s);
         else ToLog(s);
     }
     public instance_MyStringBuilder()
     {
         string s = "constructor instance_MyStringBuilder without parameters";
         if (ToLog == null) MessageBox.Show(s);
         else ToLog(s);       
     }

     public static IntPtr Zero = new IntPtr(2000);

     public StringBuilder Append(char h)
     {
         string s = "instance_MyStringBuilder Append";
         if (ToLog == null) MessageBox.Show(s);
         else ToLog(s);    
         return new StringBuilder("ho ho ho");
     }
 }

 public class instance_TypeTemplate1
 {
     public static event Addition.UI ToLog;
     public static bool ToLogisNull() { return ToLog == null; }
     public int i;
     public void Msg(string S)
     {
         string s = "instance_TypeTemplate2 Msg - " + S;
         if (ToLog == null) MessageBox.Show(s);
         else ToLog(s);       
     }
 }

 public class TestTypeTemplate
 {
     public static event Addition.UI ToLog;
     public static bool ToLogisNull(){return ToLog==null;}

     public static StringBuilder Magic(string s)
     {
         return new StringBuilder(s + IntPtr.Zero.ToString()+new IntPtr(100).ToString());
     }

     public void altdyn_TestAlternativeDynamic(object a, object b, object c)
     {
         RaiseIL.Dummy(RaiseIL.Dummy(), RaiseIL.Dummy(), RaiseIL.Dummy(), RaiseIL.Dummy(), RaiseIL.Dummy());
         Trace.WriteLine(3.ToString());
         TypeTemplate0 a0 = (TypeTemplate0)a; TypeTemplate1 a1 = (TypeTemplate1)b; TypeTemplate2 a2 = (TypeTemplate2)c;
         a0.Append('h');
         TypeTemplate0 a3 = new TypeTemplate0("testAltDyn");

         if (ToLog == null) MessageBox.Show("a3 = " + a3.ToString() + " a0 = " + a0.ToString());
         else ToLog("a3 = " + a3.ToString() + " a0 = " + a0.ToString()); 
       
         //TypeTemplate0.Zero = new IntPtr(77);

         if (ToLog == null) MessageBox.Show("Zero = " + TypeTemplate2.Zero.ToString());
         else ToLog("Zero = "+TypeTemplate2.Zero.ToString()); 

         a1.Msg("testTypeTemplate1");
     }

     [Refactoring("altdyn")]
     public void TestAlternativeDynamic(object a, object b, object c)
     {
         RaiseIL.Dummy(RaiseIL.Dummy(), RaiseIL.Dummy(), RaiseIL.Dummy(), RaiseIL.Dummy(), RaiseIL.Dummy());
         Trace.WriteLine(3.ToString());
         TypeTemplate0 a0 = (TypeTemplate0)a; TypeTemplate1 a1 = (TypeTemplate1)b; TypeTemplate2 a2 = (TypeTemplate2)c;
         a0.Append('h');
         TypeTemplate0 a3 = new TypeTemplate0("testAltDyn");

         if (ToLog == null) MessageBox.Show("a3 = " + a3.ToString() + " a0 = " + a0.ToString());
         else ToLog("a3 = " + a3.ToString() + " a0 = " + a0.ToString());

         //TypeTemplate0.Zero = new IntPtr(77);

         if (ToLog == null) MessageBox.Show("Zero = " + TypeTemplate2.Zero.ToString());
         else ToLog("Zero = " + TypeTemplate2.Zero.ToString());

         a1.Msg("testTypeTemplate1");
     }


 }
