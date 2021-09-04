/******************************************************************************
 * Qt widgets
 *
 */
Qt = :QMenuBar
{
	background-color = :palette(base)
	spacing          = :0
}

Qt = :QMenuBar::item
{
	padding             = :3 10 3 10
    background-color    = :transparent
	subcontrol-origin   = :margin
	subcontrol-position = :top left
}

Qt = :QMenuBar::item:selected
{
    background-color = :palette(dark)
}

Qt = :QMenuBar::item:pressed
{
	border              = :1 solid #34343a
	border-bottom-color = :palette(dark)
	background-color    = :palette(dark)
}

Qt = :QMenu
{
	background-color = :palette(dark)
	border           = :1 solid #34343a
	padding          = :2
}

Qt = :QMenu::icon
{
	margin = :10
}

Qt = :QMenu::item
{
	padding   = :4 12 3 20
	min-width = :180
	margin    = :0 -1 0 -1
}

Qt = :QMenu::item:selected
{
	background-color = :#28282d
}

Qt = :QMenu::item:disabled
{
	color = :#4e4e55
}

Qt = :QMenu::separator
{
    background-color = :#34343a
    height           = :1
	margin           = :2 0 2 32
}

// remove default assets
Qt = :QMenu::indicator:non-exclusive:checked
{
	background-color = :transparent
}

Qt = :QMenu::right-arrow
{
	background-color = :transparent
}

Qt = :QSplitter::handle:horizontal
{
    background-color = :palette(base)
}

Qt = :QSplitter::handle:pressed
{
    background-color = :#d15a15
}

/******************************************************************************
 * Custom widgets
 *
 */
DDNet = :TabWidget::pane
{
	border-top = :3 solid #d15a15
}

DDNet = :TabBar
{
	qproperty-drawBase = :0
}

DDNet = :TabBar::tab
{
    height  = :24
	width   = :150
	border  = :none
	padding = :0 4 0 4
}

DDNet = :TabBar::tab:hover
{
	background-color = :#a64811
}

DDNet = :TabBar::tab:selected
{
	background-color = :#d15a15
}

DDNet = :TabBar QToolButton
{
	background-color = :#313137
	border           = :none
}

DDNet = :CloseButton:hover
{
	background-color = :#16ffffff
	border           = :none
}

DDNet = :CloseButton:pressed
{
	background-color = :#10ffffff
	border           = :none
}

DDNet = :ToolBar
{
	background-color = :palette(base)
	border           = :none
	margin           = :0
	padding          = :0
}

DDNet = :ToolBar::separator
{
	background-color = :qlineargradient(x1: 0, y1: 0, x2: 1, y2: 0, stop: 0 #0b0b0f, stop: 1 palette(button))
	width            = :2
	margin           = :0 3 0 3
}

DDNet = :ToolBar QToolButton
{
	max-width  = :24
	max-height = :24
	border     = :none
	margin     = :0 2 0 2
	padding    = :0
}

DDNet = :ToolBar QToolButton:hover
{
	background-color = :#16ffffff
	border           = :none
}

DDNet = :ToolBar QToolButton:pressed
{
	background-color = :#10ffffff
	border           = :none
}
