using System;
using System.Collections.Generic;
using System.Reflection;
using System.Runtime.CompilerServices;

public class aop
{
    public object BasicO;
    public MethodInfo BeginM = null;
    public MethodInfo EndM = null;
}

public class aops
{
    public List<string> HandlersConditionsInst;    
    public List<aop> O = new List<aop>();
    public byte[] BegCode;
    public byte[] ChangeCode;
    public MethodBase BasicM;
    public MethodInfo BasicBeginM;
    public MethodInfo BasicEndM;
    public aops(MethodBase M, MethodInfo BM, MethodInfo EM)
    {
        HandlersConditionsInst = new List<string>();
        foreach (var x in AOP.HandlersConditions) if (x.Value(M)) HandlersConditionsInst.Add(x.Key);
        
        BegCode = M.GetMethodBody().GetILAsByteArray();
        ChangeCode = AOP.GetRealBody(M);
        BasicM = M;
        BasicBeginM = BM;
        BasicEndM = EM;
        RuntimeHelpers.PrepareMethod(this.BasicM.MethodHandle);
        InjectionHelper.UpdateILCodes(this.BasicM, this.ChangeCode);
    }    
    public aops(MethodBase M) : this(M, null, null)
    {
    }
}

public class aopsL : List<aops>
{
    public static bool IniAltDyn=false;
   
    public aops this[MethodBase index]
    {
        get
        {
            int i = this.FindIndex(x => x.BasicM == index);
             if (i == -1) {
                 var Res = new aops(index, null, null);
                 this.Add(Res);
                 return Res;
             }
             else return base[i];
        }
        set
        {
            int i = this.FindIndex(x => x.BasicM == index);
            if (i != -1) base[i] = value;           
        }
    }

    public aops AddM(MethodBase M, MethodInfo B, MethodInfo E)
    {
        int i = this.FindIndex(x => x.BasicM == M);
        aops r;
        if (i == -1)
        {
            r = new aops(M, B, E);
            this.Add(r);
        }
        else
        {
            r = this[i];
            this[i].BasicBeginM = B;
            this[i].BasicEndM = E;
        };
        return r;
    }  

    public aops AddM(MethodBase M)
    {
        return AddM(M, null, null);
    }

    public void AddM(List<MethodBase> M, MethodInfo B = null, MethodInfo E = null)
    {
        foreach (var m in M) AddM(m, B, E);
    }

    public void AddM(Type T, MethodInfo B = null, MethodInfo E = null)
    {
        foreach (var m in T.GetMethods(Addition.ALL)) AddM(m, B, E);
    }

    public aop AddO(MethodBase M, object O, MethodInfo B = null, MethodInfo E = null)
    {
        int i = this.FindIndex(x => x.BasicM == M);
        aops R; aop r;
        if (i == -1)
        {
            R = new aops(M, null, null);
            this.Add(R);
        }
        else
            R = this[i];
        i = R.O.FindIndex(x => x.BasicO == O);
        if (i == -1)
        {
            r = new aop() { BasicO = O, BeginM = B, EndM = E };
            R.O.Add(r);
        }
        else
        {
            r = R.O[i];
            R.O[i].BeginM = B;
            R.O[i].EndM = E;
        }
        return r;
    }

    public aops GetfromM(MethodBase M)
    {
        int i = this.FindIndex(x => x.BasicM == M);
        return (i == -1) ? null : this[i];
    }

    public aop GetfromO( MethodBase M, object O)
    {
        int i = this.FindIndex(x => x.BasicM == M);
        if (i == -1) return null;
        aops R = this[i];
        i = R.O.FindIndex(x => x.BasicO == O);
        return (i == -1) ? null : R.O[i];
    }
}
