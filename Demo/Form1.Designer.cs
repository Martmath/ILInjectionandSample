    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.btnReplaceAndCall = new System.Windows.Forms.Button();
            this.btnCallDirectly = new System.Windows.Forms.Button();
            this.btnReplaceGeneric = new System.Windows.Forms.Button();
            this.btnReplaceDynamicMethod = new System.Windows.Forms.Button();
            this.btnUpdateStaticMethod = new System.Windows.Forms.Button();
            this.button1 = new System.Windows.Forms.Button();
            this.button2 = new System.Windows.Forms.Button();
            this.txtOutput = new System.Windows.Forms.RichTextBox();
            this.button3 = new System.Windows.Forms.Button();
            this.SuspendLayout();
            // 
            // btnReplaceAndCall
            // 
            this.btnReplaceAndCall.Location = new System.Drawing.Point(1, 43);
            this.btnReplaceAndCall.Name = "btnReplaceAndCall";
            this.btnReplaceAndCall.Size = new System.Drawing.Size(216, 23);
            this.btnReplaceAndCall.TabIndex = 0;
            this.btnReplaceAndCall.Text = "Modify the IL and call \'CompareOneAndTwo\'";
            this.btnReplaceAndCall.UseVisualStyleBackColor = true;
            this.btnReplaceAndCall.Click += new System.EventHandler(this.btnReplaceAndCall_Click);
            // 
            // btnCallDirectly
            // 
            this.btnCallDirectly.Location = new System.Drawing.Point(1, 12);
            this.btnCallDirectly.Name = "btnCallDirectly";
            this.btnCallDirectly.Size = new System.Drawing.Size(216, 22);
            this.btnCallDirectly.TabIndex = 1;
            this.btnCallDirectly.Text = "Call \'CompareOneAndTwo\' method directly";
            this.btnCallDirectly.UseVisualStyleBackColor = true;
            this.btnCallDirectly.Click += new System.EventHandler(this.btnCallDirectly_Click);
            // 
            // btnReplaceGeneric
            // 
            this.btnReplaceGeneric.Location = new System.Drawing.Point(1, 96);
            this.btnReplaceGeneric.Name = "btnReplaceGeneric";
            this.btnReplaceGeneric.Size = new System.Drawing.Size(216, 22);
            this.btnReplaceGeneric.TabIndex = 2;
            this.btnReplaceGeneric.Text = "Modify Generic Method IL Code";
            this.btnReplaceGeneric.UseVisualStyleBackColor = true;
            this.btnReplaceGeneric.Click += new System.EventHandler(this.btnReplaceGeneric_Click);
            // 
            // btnReplaceDynamicMethod
            // 
            this.btnReplaceDynamicMethod.Location = new System.Drawing.Point(1, 125);
            this.btnReplaceDynamicMethod.Name = "btnReplaceDynamicMethod";
            this.btnReplaceDynamicMethod.Size = new System.Drawing.Size(216, 22);
            this.btnReplaceDynamicMethod.TabIndex = 3;
            this.btnReplaceDynamicMethod.Text = "Modify Dynamic Method IL Code";
            this.btnReplaceDynamicMethod.UseVisualStyleBackColor = true;
            this.btnReplaceDynamicMethod.Click += new System.EventHandler(this.btnReplaceDynamicMethod_Click);
            // 
            // btnUpdateStaticMethod
            // 
            this.btnUpdateStaticMethod.Location = new System.Drawing.Point(1, 156);
            this.btnUpdateStaticMethod.Name = "btnUpdateStaticMethod";
            this.btnUpdateStaticMethod.Size = new System.Drawing.Size(216, 22);
            this.btnUpdateStaticMethod.TabIndex = 4;
            this.btnUpdateStaticMethod.Text = "Modify Static Method IL Code";
            this.btnUpdateStaticMethod.UseVisualStyleBackColor = true;
            this.btnUpdateStaticMethod.Click += new System.EventHandler(this.btnUpdateStaticMethod_Click);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(1, 206);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(216, 22);
            this.button1.TabIndex = 8;
            this.button1.Text = "Test my AOP";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // button2
            // 
            this.button2.Location = new System.Drawing.Point(1, 234);
            this.button2.Name = "button2";
            this.button2.Size = new System.Drawing.Size(216, 22);
            this.button2.TabIndex = 9;
            this.button2.Text = "Test my Dynamic";
            this.button2.UseVisualStyleBackColor = true;
            this.button2.Click += new System.EventHandler(this.button2_Click);
            // 
            // txtOutput
            // 
            this.txtOutput.BackColor = System.Drawing.SystemColors.Control;
            this.txtOutput.Dock = System.Windows.Forms.DockStyle.Right;
            this.txtOutput.Font = new System.Drawing.Font("Tahoma", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(204)));
            this.txtOutput.Location = new System.Drawing.Point(216, 0);
            this.txtOutput.Name = "txtOutput";
            this.txtOutput.Size = new System.Drawing.Size(736, 630);
            this.txtOutput.TabIndex = 10;
            this.txtOutput.Text = "";
            // 
            // button3
            // 
            this.button3.Location = new System.Drawing.Point(1, 262);
            this.button3.Name = "button3";
            this.button3.Size = new System.Drawing.Size(216, 22);
            this.button3.TabIndex = 11;
            this.button3.Text = "Test IL code getting";
            this.button3.UseVisualStyleBackColor = true;
            this.button3.Click += new System.EventHandler(this.button3_Click);
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(952, 630);
            this.Controls.Add(this.button3);
            this.Controls.Add(this.txtOutput);
            this.Controls.Add(this.button2);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.btnUpdateStaticMethod);
            this.Controls.Add(this.btnReplaceDynamicMethod);
            this.Controls.Add(this.btnReplaceGeneric);
            this.Controls.Add(this.btnCallDirectly);
            this.Controls.Add(this.btnReplaceAndCall);
            this.Name = "Form1";
            this.Text = "TestInjection";
            this.Load += new System.EventHandler(this.Form1_Load);
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Button btnReplaceAndCall;
        private System.Windows.Forms.Button btnCallDirectly;
        private System.Windows.Forms.Button btnReplaceGeneric;
        private System.Windows.Forms.Button btnReplaceDynamicMethod;
        private System.Windows.Forms.Button btnUpdateStaticMethod;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.Button button2;
        public System.Windows.Forms.RichTextBox txtOutput;
        private System.Windows.Forms.Button button3;
}

