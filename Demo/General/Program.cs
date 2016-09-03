using System;
using System.Windows.Forms;

//namespace HelloWorld_V4
//{
static class Program
    {
    /// <summary>
    /// The main entry point for the application.
    /// </summary>
    
    public static Form1 F;
    [STAThread]
    static void Main()
        {
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
        F=new Form1();
            Application.Run(F);
        }
    }
//}
