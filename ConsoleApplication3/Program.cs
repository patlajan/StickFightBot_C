using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace ConsoleApplication3
{
    partial class Program
    {
        [DllImport(@"C:\Users\Elian\Documents\Visual Studio 2015\Projects\StickFightBot_C\x64\Debug\ConsoleApplication1.dll", CharSet = CharSet.Unicode)]
        public static extern int init();

        [DllImport(@"C:\Users\Elian\Documents\Visual Studio 2015\Projects\StickFightBot_C\x64\Debug\ConsoleApplication1.dll", CharSet = CharSet.Unicode)]
        public static extern int destroy();

        [DllImport(@"C:\Users\Elian\Documents\Visual Studio 2015\Projects\StickFightBot_C\x64\Debug\ConsoleApplication1.dll", CharSet = CharSet.Unicode)]
        public static extern int getNext();
        
        static void Main(string[] args)
        {
            try
            {
                var ai = new AI(TakeAction);
                init();
                System.Threading.Thread.Sleep(2000);
                for (int i = 0; i < 200; i++)
                {
                    TakeAction("{LEFT}");
                }

                while(true)
                    getNext();
            }
            finally
            {
                destroy();
            }
        }

        static void TakeAction(string action) {

            if(!string.IsNullOrEmpty(action))
                SendKeys.SendWait(action);
        }
    }
}

