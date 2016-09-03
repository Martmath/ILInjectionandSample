using System;
using System.Collections.Generic;
using System.Reflection;
using System.Reflection.Emit;


[AttributeUsage(AttributeTargets.Method)]
public class RefactoringAttribute : System.Attribute
{
    // Закрытые поля.
    private string[] refactoringType;
    public RefactoringAttribute(params string[] refactoringType)
    {
        this.refactoringType = refactoringType;
    }
    public virtual string[] RefactoringType
    {
        get { return refactoringType; }
        set { refactoringType = value; }
    }

}

public class MList : List<object>
{
    public static object threadLock = new object();
    public static MList L = new MList();
    public static MethodBase M;
    public static object O;
    public new void Add(object o)
    {
        try
        {
            base.Add(o);
        }
        catch
        {
            L.Add(o);
        }
    }
}

public static class IniALL
{
    public static bool ItRDynamic(MemberInfo M) { return ItRefact(M, "altdyn"); }
    public static bool ItRefact(MemberInfo M, string Label)
    {
        RefactoringAttribute MyAttribute =
        (RefactoringAttribute)Attribute.GetCustomAttribute(M, typeof(RefactoringAttribute));
        bool Res = MyAttribute == null ? false :
            (MyAttribute.RefactoringType.FirstOrDefault(x => x == Label) != null);
        return Res || M.Name.Contains(Label);
    }
    public static void Ini()
    {
        Globals.LoadOpCodes();
        System.Runtime.CompilerServices.RuntimeHelpers.RunClassConstructor(typeof(Description).TypeHandle);
        System.Runtime.CompilerServices.RuntimeHelpers.RunClassConstructor(typeof(AOP).TypeHandle);
        //System.Runtime.CompilerServices.RuntimeHelpers.RunClassConstructor(typeof(RealTokens).TypeHandle);       

        AOP.HandlersConditions.Add("altdyn", IniALL.ItRDynamic);
        AOP.Handlers.Add("altdyn", TypeTemplate.changeCode);

        AOP.Z.InitAllCodeModificator();
    }

    public static void InitAllCodeModificator(this aopsL Me)
    {

        if (!aopsL.IniAltDyn)
        {
            aopsL.IniAltDyn = true;
            AOP.HandlersConditions.ForEach(a =>
                Assembly.GetExecutingAssembly().GetTypes().ForEach(x =>
                x.GetMethods(Addition.ALL).Where(y => a.Value(y)).ForEach(z => Me.AddM(z))));
        }
    }
}

public static class Description
{
    public static bool It4;
    private static bool It64;
    private static  string Ver;
    public static bool SpecialVar;
    public static string GetDescription;    
    
    static Description()
    {        
        var h = typeof(AOP).Assembly.GetCustomAttributes(true).Where(x => x.ToString().Contains("TargetFrameworkAttribute"));
        if (h.Count > 0)
        {
          It4=true;
            Ver= h[0].GetType().GetProperty("FrameworkDisplayName").GetValue(h[0], null).ToString();
        }
        else
        {
            It4=false;
            var n = Assembly.GetExecutingAssembly().GetReferencedAssemblies().Where(x => x.Name == "System.Core");
            if (n.Count > 0) Ver= "<=" + n[0].Version.ToString(); else Ver="";
#if RUNNING_ON_2
            Ver=Ver +" (2.0)";
#endif
        }
        It64=System.Runtime.InteropServices.Marshal.SizeOf(typeof(IntPtr)) == 8;
        GetDescription= Ver + (It64 ? " 64" : " 32");
        SpecialVar = It64 && (!It4);
    }    
}


public static class Addition
{
    public delegate void UI(string Info);
   
    public static BindingFlags ALL = BindingFlags.Public | BindingFlags.NonPublic | BindingFlags.Instance | BindingFlags.Static;
    public static Module module = typeof(Addition).Module;
    public static byte bValue(this OpCode Me)
    {
        return (byte)Me.Value;
    }

    public static aop GetfromO(this List<aop> Me, object M)
    {
        int i = Me.FindIndex(x => x.BasicO == M);
        return (i > -1) ? Me[i] : null;
    }

    public static int ToInt32(byte[] il, int position)
    {
        return il[position] | (il[position + 1] << 8) | (il[position + 2] << 16) | (il[position + 3] << 24);
    }
    public static int ToInt32(IEnumerable<byte> IL)
    {
        int j = 0;
        int result = 0;
        foreach (var i in IL) result |= (int)(i << 8 * (j++));
        return result;
    }
    public static IEnumerable<byte> FromInt32(int intValue)
    {
        yield return (byte)intValue; yield return (byte)(intValue >> 8);
        yield return (byte)(intValue >> 16); yield return (byte)(intValue >> 24);
    }

    public static List<byte> FromInt32List(int iVal)
    {
        List<byte> Res = new List<byte>();
        for (int i = 0; i < 25; i += 8) Res.Add((byte)(iVal >> i));
        return (Res);
    }

    public static IEnumerable<byte> ReadBytes(byte[] il, int position, int length)
    {
        for (int i = position; i < position + length; i++) yield return il[i];
    }

    public static bool ItRef(Type T)
    {
        string N = T.FullName;
        return (N[N.Length - 1] == '&');
    }

    public static Type NotRef(Type T)
    {
        string N = T.FullName;
        return Type.GetType(N.Substring(0, N.Length - 1));
    }
}

public static class RaiseIL
{
    public delegate void V();
    private static void dummy()
    { }
    public static void Dummy(V v, V w, V x, V y, V z)
    { }
    public static void Dummy_(params V[] v)
    { }
    public static V Dummy()
    { return (V)dummy; }
}
