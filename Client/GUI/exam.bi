<?xml version="1.0" encoding="EUC-KR" ?>
<window name="window1" x="100" y="100" w="200" h="200" visible="0" movable="0">
	<texture file="test.tga" tx="20" ty="30" tw="40" th="10"/>
	<button name="button1" x="20" y="30" w="40" h="10" visible="0" movable="0">
		<texture file="text.tga" tx="20" ty="30" tw="40" th="10"/>
		<texture_over file="text.tga" tx="20" ty="30" tw="40" th="10"/>
		<texture_down file="text.tga" tx="20" ty="30" tw="40" th="10"/>
		<event type="L_DOWN" call="LButton_Down" />
		<event type="L_UP" call="LButton_Up" />
		<event type="ON_FOCUS" call="On_Focus" />
	</button>
	<checkbox name="checkbox1">
	</checkbox>
	<editbox name="editbox1">
	</editbox>
	<form name="form1">
	</form>
	<listbox name="listbox1">
	</listbox>
	<radiobutton name="radiobutton1">
	</radiobutton>
	<scrollbar name="scrollbar">
	</scrollbar>
	<slider name="slider1">
	</slider>
	<statictext name="statictext1">
	</statictext>
</window>