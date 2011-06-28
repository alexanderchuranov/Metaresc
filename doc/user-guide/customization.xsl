<?xml version='1.0'?>
<xsl:stylesheet
    xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  
  <!-- TODO: The location of docbook-xsl should not be hardcoded -->
  <xsl:import href="/usr/local/share/xsl/docbook/xhtml/chunk.xsl"/>
  
  <xsl:param name="use.id.as.filename" select="1"/>
  <xsl:param name="html.stylesheet">/css/metaresc/screen.css</xsl:param>
  
  <xsl:param name="local.l10n.xml" select="document('')"/>
  <l:i18n xmlns:l="http://docbook.sourceforge.net/xmlns/l10n/1.0">
    <l:l10n language="en">
      <l:context name="xref">
        <l:template name="bridgehead" text="“%t”"/>
        <l:template name="refsection" text="“%t”"/>
        <l:template name="refsect1" text="“%t”"/>
        <l:template name="refsect2" text="“%t”"/>
        <l:template name="refsect3" text="“%t”"/>
        <l:template name="sect1" text="“%t”"/>
        <l:template name="sect2" text="“%t”"/>
        <l:template name="sect3" text="“%t”"/>
        <l:template name="sect4" text="“%t”"/>
        <l:template name="sect5" text="“%t”"/>
        <l:template name="section" text="“%t”"/>
        <l:template name="simplesect" text="“%t”"/>
      </l:context>
    </l:l10n>
  </l:i18n>
  
</xsl:stylesheet>
