using System;
using System.Windows.Forms;
using De.Boenigk.SMC5D.Basics;
using De.Boenigk.SMC5D.Move;

namespace Example
{
    public partial class Form1 : Form{

        private Connector myConnector = new Connector(Application.StartupPath + "\\");

        private ManualMove myManualMove = null;

        public Form1() {
            InitializeComponent();

            myManualMove = new ManualMove(myConnector);
            comboBox1.SelectedIndex = 0;
        }

        private void buttonConnect_Click(object sender, EventArgs e) {
            if (myConnector.Load(Application.StartupPath + "\\config.xml")) {
                labelInfo.Text = "Configuration loaded";
            }
        }

        private void button1_Click(object sender, EventArgs e) {
            myConnector.Enable(0);
        }

        private void timerStatus_Tick(object sender, EventArgs e) {

            if (myConnector.Status == SMCStatus.Disabled) {
                label1.Text = "Connection closed";
                return;
            }

            if (myConnector.Status != SMCStatus.NotFound) {
                label1.Text = "Connected";
                label3.Text = "X " + Math.Round(StepCalc.GetMMX(myConnector, myConnector.GlobPosition.X), 3);
                label4.Text = "Y " + Math.Round(StepCalc.GetMMY(myConnector, myConnector.GlobPosition.Y), 3);
                label5.Text = "Z " + Math.Round(StepCalc.GetMMZ(myConnector, myConnector.GlobPosition.Z), 3);

                label6.Text = myConnector.GlobPosition.Out.ToString();
            } else {
                label1.Text = "No connection!";
            }
        }

        private void button2_Click(object sender, EventArgs e) {
            myConnector.Disable();
        }

        private void button3_Click(object sender, EventArgs e) {
            Reference theReference = new Reference(myConnector);
            theReference.Start();
        }

        private void button4_Click(object sender, EventArgs e) {
            myConnector.Abort();
        }

        private void button5_Click(object sender, EventArgs e) {
            StepList theStepList = new StepList(myConnector.GlobPosition, myConnector.SMCSettings);

            Step theStep = new Step();
            theStep.X = 100;
            theStep.Y = 100;
            theStep.Z = StepCalc.GetMMZ(myConnector, myConnector.GlobPosition.Z);
            theStep.Speed = 10;

            theStepList.Add(theStep);

            theStep.X = 200;
            theStepList.Add(theStep);

            theStep.Y = 200;
            theStepList.Add(theStep);

            theStep.X = 100;
            theStepList.Add(theStep);

            theStep.Y = 100;
            theStepList.Add(theStep);

            Job theJob = new Job(myConnector);
            theJob.SetStepList(theStepList.List);

            theJob.Start(100);
        }

        private void button6_Click(object sender, EventArgs e) {

            int theNr = Int32.Parse(comboBox1.Text);
            Output theOutput = OutputConverter.GetOutput(theNr);

            Switch theSwitch = new Switch(myConnector);
            theSwitch.SetPin(theOutput, true);
        }

        private void button7_Click(object sender, EventArgs e) {

            int theNr = Int32.Parse(comboBox1.Text);
            Output theOutput = OutputConverter.GetOutput(theNr);

            Switch theSwitch = new Switch(myConnector);
            theSwitch.SetPin(theOutput, false);
        }

        private void button8_Click(object sender, EventArgs e) {
            SMCInfo theSMCInfo = new SMCInfo(myConnector);
            label2.Text = theSMCInfo.GetSNString();
        }

        private const int WM_DEVICECHANGE = 0x219;

        /// <summary>
        /// Baut die Verbindung wieder auf wenn Kontroller ausgesteckt und wieder eingesteckt wird
        /// </summary>
        /// <param name="msg"></param>
        protected override void WndProc(ref Message msg) {
            if (msg.Msg == WM_DEVICECHANGE)
            {
                if (!myConnector.Enabled)
                {
                    myConnector.Enable(0);
                }
            }
            base.WndProc(ref msg);
        }

        private void buttonMoveXP_MouseUp(object sender, MouseEventArgs e)
        {
            myManualMove.Stop();
        }

        private float GetSpeed() {
            try {
                float theSpeed = (float) Double.Parse(textBox1.Text);
                return theSpeed;
            } catch {
                
            }
            return 10;
        }

        private void buttonMoveXP_MouseDown(object sender, MouseEventArgs e)
        {
            myManualMove.Step(MoveAxis.X, true, GetSpeed());
        }

        private void buttonMoveYP_MouseDown(object sender, MouseEventArgs e)
        {
            myManualMove.Step(MoveAxis.Y, true, GetSpeed());
        }

        private void buttonMoveXM_MouseDown(object sender, MouseEventArgs e)
        {
            myManualMove.Step(MoveAxis.X, false, GetSpeed());
        }

        private void buttonMoveYM_MouseDown(object sender, MouseEventArgs e)
        {
            myManualMove.Step(MoveAxis.Y, false, GetSpeed());
        }

        private void buttonMoveZP_MouseDown(object sender, MouseEventArgs e)
        {
            myManualMove.Step(MoveAxis.Z, true, GetSpeed());
        }

        private void buttonMoveZM_MouseDown(object sender, MouseEventArgs e)
        {
            myManualMove.Step(MoveAxis.Z, false, GetSpeed());
        }

        private void buttonMoveXPYP_MouseDown(object sender, MouseEventArgs e)
        {
            myManualMove.StepXY(true, true, GetSpeed());
        }

        private void buttonMoveXPYM_MouseDown(object sender, MouseEventArgs e)
        {
            myManualMove.StepXY(true, false, GetSpeed());
        }

        private void buttonMoveXMYM_MouseDown(object sender, MouseEventArgs e)
        {
            myManualMove.StepXY(false, false, GetSpeed());
        }

        private void buttonMoveXMYP_MouseDown(object sender, MouseEventArgs e)
        {
            myManualMove.StepXY(false, true, GetSpeed());
        }

    }
}
