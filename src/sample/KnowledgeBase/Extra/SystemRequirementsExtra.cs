﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Cosmos;

namespace KnowledgeBase
{
    public partial class SystemRequirementsExtra : UserControl
    {
        private Grid thisGrid = null;

        public SystemRequirementsExtra()
        {
            InitializeComponent();
        }

        private void SystemRequirementsExtra_Load(object sender, EventArgs e)
        {
            thisGrid = Hubble.CreatingGrid;
            if (thisGrid != null)
            {
                thisGrid.OnCloudMessageReceived += ThisNode_OnCloudMessageReceived;
            }
        }

        private void ThisNode_OnCloudMessageReceived(Wormhole cloudWormhole)
        {
            if (cloudWormhole.GetString("msgID").Equals("system_requirements_extra_update_label"))
            {
                string labelText = cloudWormhole.GetString("label_text");
                label2.Text = labelText;
            }
        }
    }
}
