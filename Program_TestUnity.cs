using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Reflection.Emit;
using System.Threading;
namespace Test
{
	class MainClass
	{

		[DllImport("kernel32.dll", SetLastError = true, ExactSpelling = true, CharSet = CharSet.Unicode)]
		private static extern IntPtr LoadLibraryW(string lpFileName);

		[DllImport("kernel32.dll", SetLastError = true, ExactSpelling = true)]
		private static extern bool FreeLibrary(IntPtr hModule);

		[DllImport("kernel32.dll", CharSet = CharSet.Ansi, ExactSpelling = true, SetLastError = true)]
		private static extern IntPtr GetProcAddress(IntPtr hModule, string procName);

		[UnmanagedFunctionPointer(CallingConvention.StdCall)]
		private delegate bool UpdateILCodesDelegate(IntPtr pMethodTable, IntPtr pMethodHandle, int md, IntPtr pBuffer, int dwSize);
		private static UpdateILCodesDelegate _updateILCodesMethod;

		public static void Main (string[] args)
		{	
			string currentFile = Path.GetDirectoryName(Assembly.GetExecutingAssembly().CodeBase);

			currentFile = Path.Combine(currentFile.Substring(6), (IntPtr.Size == 8) ? "Injection64.dll" : "Injection32.dll");
			IntPtr _moduleHandle = LoadLibraryW(currentFile);
			if (_moduleHandle == IntPtr.Zero)
				throw new FileNotFoundException( string.Format( "Failed to load [{0}]", currentFile) );

			IntPtr ptr = GetProcAddress(_moduleHandle, "UpdateILCodes");
			if (ptr == IntPtr.Zero)
				throw new MethodAccessException("Failed to locate UpdateILCodes function!");
			_updateILCodesMethod = (UpdateILCodesDelegate)Marshal.GetDelegateForFunctionPointer(ptr, typeof(UpdateILCodesDelegate));

			Type type = typeof (MainClass);
			MethodInfo methodInfo = type.GetMethod("CompareOneAndTwo", BindingFlags.NonPublic | BindingFlags.Static);
			RuntimeHelpers.PrepareMethod(methodInfo.MethodHandle);	
			byte[] ilCodes = methodInfo.GetMethodBody().GetILAsByteArray();        
			//for (int i = 0; i < ilCodes.Length; i++) Console.WriteLine(ilCodes [i].ToString());	
			for (int i = 0; i < ilCodes.Length; i++)
			{
				if ((ilCodes [i] == OpCodes.Bge_S.Value)) {  
					Console.WriteLine(i.ToString());
					ilCodes [i] = (byte)OpCodes.Blt_S.Value;
				} 
				if ((ilCodes [i] ==OpCodes.Bge.Value)) {  
					Console.WriteLine(i.ToString());
					ilCodes [i] = (byte)OpCodes.Blt.Value;
				} 
			}  

			Console.WriteLine(ilCodes.Length.ToString());	Console.ReadKey();
			UpdateILCodes(methodInfo, ilCodes);

			Console.WriteLine(string.Format("The result is: \"{0}\"", CompareOneAndTwo())); Console.ReadKey();

			//if (_moduleHandle != IntPtr.Zero)	FreeLibrary(_moduleHandle);
		}

		public static void UpdateILCodes(MethodBase method, byte[] ilCodes)
		{
			if (_updateILCodesMethod == null)
				throw new Exception("Please Initialize() first.");

			IntPtr pMethodTable = IntPtr.Zero;
			if (method.DeclaringType != null)
				pMethodTable = method.DeclaringType.TypeHandle.Value;
			
			IntPtr pMethodHandle = method.MethodHandle.Value;
			IntPtr pBuffer = Marshal.AllocHGlobal(ilCodes.Length);
			if (pBuffer == IntPtr.Zero)
				throw new OutOfMemoryException();
			Marshal.Copy(ilCodes, 0, pBuffer, ilCodes.Length);
			int token = 0;
			try
			{
				token = method.MetadataToken;
			}
			catch {
			} 		
			if (!_updateILCodesMethod(pMethodTable, pMethodHandle, token, pBuffer, ilCodes.Length))
				throw new Exception("UpdateILCodes() failed, please check the initialization is failed or uncompleted.");
		}

		protected static string CompareOneAndTwo()
		{
			int a = 1;	int b = 2;
			if (a < b)
				return "Number 1 is less than 2";
			else
				return "Number 1 is greater than 2 (O_o)";
		}
	}
}
