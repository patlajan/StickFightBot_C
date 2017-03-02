using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Forms;

namespace ConsoleApplication3
{
    partial class Program
    {
        [DllImport(@"C:\Users\Elian\Documents\Visual Studio 2015\Projects\StickFightBot_C\x64\Debug\StickFightCV.dll", CharSet = CharSet.Unicode)]
        public static extern int init();

        [DllImport(@"C:\Users\Elian\Documents\Visual Studio 2015\Projects\StickFightBot_C\x64\Debug\StickFightCV.dll", CharSet = CharSet.Unicode)]
        public static extern int destroy();

        [DllImport(@"C:\Users\Elian\Documents\Visual Studio 2015\Projects\StickFightBot_C\x64\Debug\StickFightCV.dll", CharSet = CharSet.Unicode)]
        public static extern IntPtr getNext(out int len);
        
        static void Main(string[] args)
        {
            try
            {
                var ai = new AI(TakeAction);
                init();
                while (true)
                {
                    //var watch = System.Diagnostics.Stopwatch.StartNew();
                    int len;
                    IntPtr ptr = getNext(out len);
                    int[] result = new int[len];
                    Marshal.Copy(ptr, result, 0, len);
                    Console.WriteLine(result[0]);

                    //watch.Stop();
                    //Console.WriteLine(watch.ElapsedMilliseconds);
                }
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

        static void RestartGame()
        {
            SendKeys.SendWait("s");
        }

        static void ParseState(int[] state)
        {
            //foreach var i in state
            //first == game state, last 4 = good guy
        }
    }
}

