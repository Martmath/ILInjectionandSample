using System;
using System.Reflection;
using System.Reflection.Emit;
using System.Collections.Generic;

public class AOP
{
    public static aopsL Z;
    public static bool It2015;
    public static Int32 TokenGetCurrentMethod;
    public static Int32 TokenInt32;
    public static Int32 TokenString;
    private static byte[] MyBody;    
    private static IEnumerable<byte> TokenGetCurrentMethodIE;    
    private static IEnumerable<byte> ToBodyBegin;
    private static IEnumerable<byte> ToBodyAdd;
    private static IEnumerable<byte> ToBodyRunDo;
    private static Type Void;

    public delegate byte[] dGetRealBody(MethodBase M);
    public delegate byte[] dHandlers(byte[] B, object Me, aops A, List<object> O);

    public static Dictionary<string, dHandlers> Handlers;
    public static Dictionary<string, Predicate<MemberInfo>> HandlersConditions;   

    public static dGetRealBody GetRealBody;    

    static AOP()
    {        
        HandlersConditions = new Dictionary<string,Predicate<MemberInfo>>();
        Handlers = new Dictionary<string, dHandlers>();   
        
        if (Description.SpecialVar)   GetRealBody = GetRealBody0;
        else GetRealBody = GetRealBody1;
        
        Void = typeof(void);
        Z = new aopsL();
        MyBody = typeof(AOP).GetMethod("Prepared", BindingFlags.NonPublic | BindingFlags.Instance).GetMethodBody().GetILAsByteArray();
        TokenGetCurrentMethod = Addition.ToInt32(MyBody, 2);
        TokenGetCurrentMethodIE = Addition.ReadBytes(MyBody, 2, 4);
        ToBodyBegin = Addition.ReadBytes(MyBody, 1, 10);
        
        It2015 = MyBody[11] == 37;//dup
        if (It2015)
        {
            TokenInt32 = Addition.ToInt32(MyBody, 14);
            ToBodyAdd = Addition.ReadBytes(MyBody, 18, 5);
            TokenString = Addition.ToInt32(MyBody, 25);
            ToBodyRunDo = Addition.ReadBytes(MyBody, 34, 5);
        }
        else {
            TokenInt32 =  Addition.ToInt32(MyBody, 15);  
            ToBodyAdd = Addition.ReadBytes(MyBody, 19, 5);
            TokenString = Addition.ToInt32(MyBody, 26);
            ToBodyRunDo = Addition.ReadBytes(MyBody, 36, 5);
        }    
    }
    
    private object Prepared(int o)
    {
        return AOP.Do(this, MethodBase.GetCurrentMethod(), new MList() { o, "test"}); 
    /*-2012 0000 : ldarg.0  (2) 
            0001 : call System.Reflection.MethodBase System.Reflection.MethodBase::GetCurrentMethod()  (40|135|0|0|10) 167772295
            0006 : newobj instance void MList::.ctor()  (115|36|0|0|6) 100663332
            0011 : stloc.0  (10) 
            0012 : ldloc.0  (6) 
            0013 : ldarg.1  (3) 
            0014 : box int  (140|55|0|0|1) 16777271
            0019 : callvirt instance System.Void MList::Add()  (111|35|0|0|6) 100663331
            0024 : ldloc.0  (6) 
            0025 : ldstr "test1879052150"  (114|118|15|0|112) 1879052150
            0030 : callvirt instance System.Void MList::Add()  (111|35|0|0|6) 100663331
            0035 : ldloc.0  (6) 
            0036 : call System.Object AOP::Do()  (40|75|0|0|6) 100663371
            0041 : ret  (42) 
    */
    }

    private static byte[] GetRealBody0(MethodBase M)
    {
        List<byte> Res = new List<byte>();
        if (!M.IsStatic)
        {
            Res.Add(OpCodes.Ldarg_0.bValue());
            Res.Add(OpCodes.Stsfld.bValue());
            Res.AddRange(typeof(MList).
                GetField("O", BindingFlags.Static | BindingFlags.Public).RealTokenL());
        }

        Res.Add(OpCodes.Call.bValue());
        Res.AddRange(TokenGetCurrentMethodIE);
        Res.Add(OpCodes.Stsfld.bValue());
        Res.AddRange(typeof(MList).
            GetField("M", BindingFlags.Static | BindingFlags.Public).RealTokenL());        
        var PS = M.GetParameters();
        List<Type> InputT = new List<Type>();
        foreach (var p in PS) InputT.Add(p.ParameterType);
        if (InputT.Count > 0)
        {
            int hh = 0;
            foreach (var t in InputT)
            {
                Res.Add(OpCodes.Ldsfld.bValue());
                Res.AddRange(typeof(MList).
                    GetField("L", BindingFlags.Static | BindingFlags.Public).RealTokenL());
                Res.AddRange(ToBodyNumArg(hh, M.IsStatic));
                Type T;
                if (Addition.ItRef(t))
                {
                    T = Addition.NotRef(t);
                    Res.Add(ToBodyifRef(T));
                }
                else T = t;
                if (T.IsValueType)
                {
                    var Token = Addition.FromInt32(T.RealToken());
                    if (Res[Res.Count - 1] == OpCodes.Ldobj.bValue()) Res.AddRange(Token);
                    Res.Add(OpCodes.Box.bValue());
                    Res.AddRange(Token);
                }
                Res.AddRange(ToBodyAdd);
                hh++;
            }          
        }
        Res.Add(OpCodes.Call.bValue());
        Res.AddRange(Addition.FromInt32(typeof(AOP).
            GetMethod((M.IsStatic ? "S" : "") + "DoS", Addition.ALL).RealToken()));

        if ((M as MethodInfo).ReturnType == Void) Res.Add(OpCodes.Pop.bValue());
        Res.Add(OpCodes.Ret.bValue());
        return Res.ToArray();
    }

    private static byte[] GetRealBody1(MethodBase M)
    {
        List<byte> Res = new List<byte>();
        if (!M.IsStatic)
        {
          //  if (!It2015) Res.Add(OpCodes.Ldarg_0.bValue());//need if Do not static
            Res.Add(OpCodes.Ldarg_0.bValue());
        }
        Res.AddRange(ToBodyBegin);
        var PS = M.GetParameters();
        List<Type> InputT = new List<Type>();
        foreach (var p in PS) InputT.Add(p.ParameterType);
        if (InputT.Count > 0)
        {
            if (!It2015) Res.Add((byte)OpCodes.Stloc_0.Value);
            int hh = 0;
            foreach (var t in InputT)
            {
               if (It2015)  Res.Add(37); else Res.Add(OpCodes.Ldloc_0.bValue());//dup  (37) 
                Res.AddRange(ToBodyNumArg(hh,M.IsStatic));
                Type T;
                if (Addition.ItRef(t))
                {
                    T = Addition.NotRef(t);
                    Res.Add(ToBodyifRef(T));
                }
                else T = t;
                if (T.IsValueType)
                {
                    var Token = Addition.FromInt32(T.RealToken());  
                    if (Res[Res.Count - 1] == OpCodes.Ldobj.bValue()) Res.AddRange(Token);
                    Res.Add(OpCodes.Box.bValue());
                    Res.AddRange(Token);
                }
                Res.AddRange(ToBodyAdd);
                hh++;
            }
            if (!It2015) Res.Add(OpCodes.Ldloc_0.bValue());          
        }
        Res.Add(OpCodes.Call.bValue()); 
        Res.AddRange(Addition.FromInt32(typeof(AOP).
            GetMethod((M.IsStatic ? "S" : "" )+ "Do", Addition.ALL).RealToken()));
       // Res.AddRange(ToBodyRunDo);
        if ((M as MethodInfo).ReturnType == Void) Res.Add(OpCodes.Pop.bValue());
        Res.Add(OpCodes.Ret.bValue());
        return Res.ToArray();
    }  

    private static IEnumerable<byte> ToBodyNumArg(int N, bool ItStatic)
    {
        if (!ItStatic) N = N + 1;
        switch (N)
        {
            case 0:
                yield return OpCodes.Ldarg_0.bValue();
                break;
            case 1:
                yield return OpCodes.Ldarg_1.bValue();
                break;
            case 2:
                yield return OpCodes.Ldarg_2.bValue();
                break;
            case 3:
                yield return OpCodes.Ldarg_3.bValue();
                break;
            default:
                yield return OpCodes.Ldarg_S.bValue();
                break;
        }
        if (N>3) yield return (byte)(N);
    }

    private static byte ToBodyifRef(Type T)
    {
        OpCode il;
        if (!T.IsValueType)
            il = OpCodes.Ldind_Ref;
        else if ((T == typeof(System.Boolean)) || (T == typeof(System.SByte)))
            il = OpCodes.Ldind_I1;
        else if (T == typeof(System.Byte))
            il = OpCodes.Ldind_U1;
        else if (T == typeof(System.Int16))
            il = OpCodes.Ldind_I2;
        else if ((T == typeof(System.UInt16)) || (T == typeof(System.Char)))
            il = OpCodes.Ldind_U2;
        else if (T == typeof(System.Int32))
            il = OpCodes.Ldind_I4;
        else if (T == typeof(System.UInt32))
            il = OpCodes.Ldind_U4;
        else if ((T == typeof(System.Int64)) || (T == typeof(System.UInt64)))
            il = OpCodes.Ldind_I8;
        else if (T == typeof(System.Double))
            il = OpCodes.Ldind_R8;
        else if (T == typeof(System.Single))
            il = OpCodes.Ldind_R4;
        else if (It2015 &&((T == typeof(System.IntPtr))|| (T == typeof(System.UIntPtr))))
            return 77;  //ldind.i  (77) 
        else
            il = OpCodes.Ldobj;
             return (byte)il.Value;     
    }

    public static object DoS()
    {
        object O; MethodBase M; List<object> L;
        lock (MList.threadLock)
        {
            O = MList.O;
            M = MList.M;
            L = MList.L.Select(x => x);
            MList.L.Clear();
        }
        return Do(O, M, L);
    }

    public static object SDoS()
    {
        MethodBase M; List<object> L;
        lock (MList.threadLock)
        {
            M = MList.M;
            L = MList.L.Select(x => x);
            MList.L.Clear();
        }
        return SDo(M, L);
    }
    public static object Do(object Me, MethodBase M, List<object> O)
    {

        if ((!Description.SpecialVar) && (MList.L.Count > 0))
        {
            lock (MList.threadLock)
            {
                O = MList.L.Select(x => x);
                MList.L.Clear();
            }
        }
        var e = Z.GetfromM(M);
        var u = e.O.GetfromO(Me);
        MethodInfo BM, EM;
        if (u == null)
        {
            BM = e.BasicBeginM;
            EM = e.BasicEndM;
        }
        else
        {
            BM = u.BeginM;
            EM = u.EndM;
        }
        if (BM != null) BM.Invoke(null, new object[] { Me, M, O });       

        byte[] R = e.BegCode;
        e.HandlersConditionsInst.ForEach(x => R = AOP.Handlers[x](R, Me, e, O));        
        InjectionHelper.UpdateILCodes(M, R);
        object Res = M.Invoke(Me, O.ToArray());
        // InjectionHelper.UpdateILCodes(M, this.GetType().GetMethod("ORunner_" + O.Count.ToString()).GetMethodBody().GetILAsByteArray());
        InjectionHelper.UpdateILCodes(M, e.ChangeCode);
        if (EM != null) EM.Invoke(null, new object[] { Res, Me, M, O }); 
        return Res;
    }
    public static object SDo(MethodBase M, List<object> O)
    {     
        if ((!Description.SpecialVar) && (MList.L.Count > 0))
        {
            lock (MList.threadLock)
            {
                O = MList.L.Select(x => x);
                MList.L.Clear();
            }
        }
        var e = Z.GetfromM(M); 
        MethodInfo BM = e.BasicBeginM;
        MethodInfo EM = e.BasicEndM;       
        if (BM != null) BM.Invoke(null, new object[] { null, M, O });
        byte[] R = e.BegCode;
        e.HandlersConditionsInst.ForEach(x=> R = AOP.Handlers[x](R, null, e, O));
       // if (e.itDynamic) InjectionHelper.UpdateILCodes(M, TypeTemplate.changeCode(e.BegCode, null, e, O));  else 
        InjectionHelper.UpdateILCodes(M, R);
        object Res = M.Invoke(null, O.ToArray());       
        InjectionHelper.UpdateILCodes(M, e.ChangeCode);
        if (EM != null) EM.Invoke(null, new object[] { Res, null, M, O }); 
        return Res;
    }
}
