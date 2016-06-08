using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Reflection;
using System.Reflection.Emit;
using System.Text;
using System.Windows.Forms;

public static class TypeTemplate
{
    public static V searchEntity<V>(V Entity, Type T) where V : MemberInfo
    {
        if (Entity is Type) return (V)(MemberInfo)T;
        string Ident = Entity.FullTypeName();
        Func<MemberInfo[]> Selector;
        if (Entity is MethodInfo)
        {  Selector = () =>T.GetMethods(Addition.ALL);}
        else if (Entity is ConstructorInfo)
        {Selector = () =>T.GetMembers(Addition.ALL);}
        else //if (Entity is FieldInfo)
        {Selector = () =>T.GetFields(Addition.ALL);}  
        var SelectotInstance = Selector();
        List<MemberInfo> Entities = SelectotInstance.Where(x => x.FullTypeName() == Ident);
        if (Entities.Count == 0) return null;
        else if (Entities.Count == 1) return (V)Entities[0];
        Ident = Entity.Name;
        return (V)Entities.FirstOrDefault(x => x.Name.Contains(Ident));
    }
   
    public static void changeEntity<V>(List<byte> Q, ILInstruction IL, Type T) where V : MemberInfo
    {
        var q = searchEntity((V)IL.Operand, T);
        if (q == null) throw new ArgumentNullException("","Not Found equal method or field");
        int i = IL.Offset;
        foreach (var x in q.RealTokenL()) Q[++i] = x;
    }

    public static void changeEntities<T>(MethodBodyReader N, Predicate<OpCode> P, Dictionary<Type, List<Type>> R, List<Type> Tnew, int i, List<byte> Q) where T : MemberInfo
    {       
        N.instructions.FindAll(x => (P(x.Code) &&
            (R[Tnew[i]].Contains((((MemberInfo)x.Operand).DeclaringType))))).
            ForEach(x => changeEntity<T>(Q, x, Tnew[i])); 
    }

    public static byte[] changeCode(byte[] Before, object Me, aops A, List<object> O)
    {
        List<Type> Tnew = O.Select(x=>x.GetType());
        List<byte> Q = new List<byte>(Before);
        var N = new MethodBodyReader(Addition.module, Before);
        List<ILInstruction> CastClass=N.instructions.FindAll(x => x.Code == OpCodes.Castclass);
        if (CastClass.Count > Tnew.Count) CastClass.RemoveRange(Tnew.Count, CastClass.Count - Tnew.Count);
        var Told = CastClass.Select(x => (Type)x.Operand);
        Dictionary<Type, List<Type>> Compare = new Dictionary<Type, List<Type>>();
        List<Type> L;
        for (int i = 0; i < Tnew.Count; i++) 
        {
            if (Compare.TryGetValue(Tnew[i], out L))
            {
                if (!L.Contains(Told[i])) L.Add(Told[i]);
            }
            else
            {
                Compare.Add(Tnew[i], new List<Type>());
                Compare[Tnew[i]].Add(Told[i]);       
            }
        }

        Predicate<OpCode> M = (x) => ((x == OpCodes.Call) || (x == OpCodes.Calli) || (x == OpCodes.Callvirt) );
        Predicate<OpCode> C = (x) =>  (x == OpCodes.Newobj);
        Predicate<OpCode> F = (x) => ((x == OpCodes.Stsfld) || (x == OpCodes.Stfld) || (x == OpCodes.Ldsfld) ||
            (x == OpCodes.Ldfld) || (x == OpCodes.Ldsflda) || (x == OpCodes.Ldflda)); 

        for (int i = 0; i < Tnew.Count; i++)
        {
            changeEntities<MethodInfo>(N, M, Compare, Tnew, i, Q);
            changeEntities<ConstructorInfo>(N, C, Compare, Tnew, i, Q);
            changeEntities<FieldInfo>(N, F, Compare, Tnew, i, Q);
            changeEntity<Type>(Q, CastClass[i], Tnew[i]);    
        }
        /* var I = new MethodBodyReader(Addition.module, Q.ToArray());
           MessageBox.Show(N.GetBodyCode(Before) + I.GetBodyCode(Q.ToArray()));*/
        return Q.ToArray();
    }
}
