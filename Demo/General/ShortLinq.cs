using System;
using System.Collections.Generic;
using System.Reflection;

#if RUNNING_ON_2
    namespace System.Runtime.CompilerServices 
    {
        public class ExtensionAttribute : Attribute { }
    }
#endif
    public delegate TResult Func<in T, out TResult>(T arg);
    public delegate TResult Func<out TResult>();
    public delegate void Action();
    public delegate void Action<T,K>(T t,K k);

public static class ShortLinq//for net 2.0
{
    public static string FullTypeName(this MemberInfo Me)
    {
        string Res = "";
        if (Me is MethodInfo)
        {
            (Me as MethodInfo).GetParameters().Select(x => x.ParameterType.FullName).ForEach(x => Res = Res + x + "->");
            Res = Res + (Me as MethodInfo).ReturnType.FullName;
        }
        else if (Me is ConstructorInfo)
            (Me as ConstructorInfo).GetParameters().Select(x => x.ParameterType.FullName).ForEach(x => Res = Res + x + "->");
        else if (Me is FieldInfo)
            Res = (Me as FieldInfo).FieldType.FullName;
        return Res;
    }

    public static void ForEach<T>(this IEnumerable<T> Me, Action<T> t)
    {
        foreach (var a in Me) t(a);
    }

    public static List<K> Select<T, K>(this IEnumerable<T> Me, Func<T, K> t)
    {
        List<K> Res = new List<K>();
        foreach (var a in Me) Res.Add(t(a));
        return Res;
    }

    public static List<T> Where<T>(this IEnumerable<T> Me, Predicate<T> t)
    {
        List<T> Res = new List<T>();
        foreach (var a in Me) if (t(a)) Res.Add(a);
        return Res;
    }

    public static T FirstOrDefault<T>(this IEnumerable<T> Me, Predicate<T> t)
    {     
        foreach (var o in Me) if (t(o)) return o;
        return default(T);
    }

    public static T FirstOrDefault<T>(this List<T> Me, int StartIndex, Predicate<T> t)
    {
        int i = Me.FindIndex(StartIndex, t);
        return i == -1 ? default(T) : Me[i];
    }
}
