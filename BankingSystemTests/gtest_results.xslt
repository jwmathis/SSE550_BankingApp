<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">
  <xsl:output method="html" indent="yes" />

  <xsl:template match="/testsuites">
    <html>
      <head>
        <title>Google Test Results</title>
      </head>
      <body>
        <h1>Google Test Results</h1>
        <ul>
          <xsl:for-each select="testsuite/testcase">
            <li>
              <xsl:value-of select="@name" />
              <xsl:if test="failure"> - Failed</xsl:if>
              <xsl:if test="not(failure)"> - Passed</xsl:if>
            </li>
          </xsl:for-each>
        </ul>
      </body>
    </html>
  </xsl:template>
</xsl:stylesheet>
