using System;
using De.Boenigk.SMC5D.Basics;
using De.Boenigk.SMC5D.Move;
using static System.Net.Mime.MediaTypeNames;

namespace cnc
{
    public partial class CNC
    {
        private Connector myConnector = new Connector(".\\");
        private ManualMove myManualMove = null;
        public bool connect()
        {
            if (!myConnector.Load(".\\config.xml"))
            {
                Console.WriteLine("Not happening.");
                return false;
            }

            myManualMove = new ManualMove(myConnector);
            Console.Write("Configuration loaded. Trying to connect.");

            SMCStatus result = myConnector.Enable();



            int timeout = 5000;

            while (myConnector.Status != SMCStatus.Ready && timeout > 0)
            {
                Thread.Sleep(50);
                timeout -= 50;
                Console.Write('.');
            }
            if (myConnector.Status != SMCStatus.Ready)
            {
                Console.WriteLine("");
                Console.WriteLine("Timeout with error {0:D} ({0:G}).", myConnector.Status);
                return false;
            }

            Console.WriteLine("");

            return true;

        }

        public void do_job()
        {
            StepList theStepList = new StepList(myConnector.GlobPosition, myConnector.SMCSettings);

            Step theStep = new Step();
            theStep.X = 10;
            theStep.Y = 10;
            theStep.Z = StepCalc.GetMMZ(myConnector, myConnector.GlobPosition.Z);
            theStep.Speed = 10;

            theStepList.Add(theStep);

            theStep.X = 20;
            theStepList.Add(theStep);

            theStep.Y = 20;
            theStepList.Add(theStep);

            theStep.X = 10;
            theStepList.Add(theStep);

            theStep.Y = 10;
            theStepList.Add(theStep);

            Job theJob = new Job(myConnector);
            theJob.SetStepList(theStepList.List);

            theJob.Start(100);

        }


    }

}

namespace Application
{
    class Program
    {
        static void Main(string[] args)
        {
            // See https://aka.ms/new-console-template for more information
            Console.WriteLine("Hello, World! Doing some smc5d library stuff now");

            cnc.CNC device = new cnc.CNC();
            if (!device.connect())
            {
                Console.WriteLine("No luck.");
                return;
            }            

            Console.WriteLine("Connection successful.");

            device.do_job();


        }
    }
}

