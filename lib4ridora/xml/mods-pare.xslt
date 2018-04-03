<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0"
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:mods="http://www.loc.gov/mods/v3">
  <xsl:output method="xml" indent="yes"/>

  <xsl:template match="mods:mods">
    <xsl:copy>
      <xsl:copy-of select="@*"/>
      <xsl:apply-templates select="mods:titleInfo[not(@type)] | mods:name | mods:originInfo | mods:relatedItem | mods:genre | mods:identifier"/>
    </xsl:copy>
  </xsl:template>

  <xsl:template match="mods:mods/mods:identifier[@type='doi'] | mods:mods/mods:genre | mods:mods/mods:originInfo">
    <xsl:call-template name="copy"/>
  </xsl:template>

  <xsl:template match="mods:titleInfo">
    <xsl:call-template name="copy">
      <xsl:with-param name="select" select="mods:title"/>
    </xsl:call-template>
  </xsl:template>
  <xsl:template match="mods:mods/mods:name">
    <xsl:call-template name="copy">
      <xsl:with-param name="select" select="mods:namePart"/>
    </xsl:call-template>
  </xsl:template>
  <xsl:template match="mods:mods/mods:relatedItem[@type='host']">
    <xsl:call-template name="copy">
      <xsl:with-param name="select" select="mods:titleInfo[not(@type)]"/>
    </xsl:call-template>
  </xsl:template>

  <xsl:template name="copy">
    <xsl:param name="select" select="text() | *"/>

    <xsl:copy>
      <xsl:copy-of select="@*"/>
      <xsl:apply-templates select="$select" mode="copied"/>
    </xsl:copy>
  </xsl:template>

  <xsl:template match="text()"/>
  <xsl:template match="*" mode="copied"/>
  <xsl:template match="mods:titleInfo | mods:title | mods:namePart | mods:dateIssued" mode="copied">
    <xsl:call-template name="copy"/>
  </xsl:template>
</xsl:stylesheet>
