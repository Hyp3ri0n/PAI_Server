<?xml version="1.0" encoding="UTF-8"?>

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0"
                xmlns:java="http://xml.apache.org/xslt/java"
                exclude-result-prefixes="java">
    <xsl:output method="html"/>

    <xsl:param name="destinedId" select="001"/>
    <xsl:param name="dateJour" select="'2015-12-08'"/>
    <!--<xsl:param name="dateJour" select="java:format(java:java.text.SimpleDateFormat.new('yyyy-MM-dd'), java:java.util.Date.new())"/>-->
    <xsl:template match="/">
        <html>
            <head>
                <title>PAI - Page Infirmière</title>
                <script type="text/javascript">
                    <![CDATA[
                        function openFacture(prenom, nom, actes) {
                            var width  = 500;
                            var height = 300;
                            if(window.innerWidth) {
                                var left = (window.innerWidth-width)/2;
                                var top = (window.innerHeight-height)/2;
                            }
                            else {
                                var left = (document.body.clientWidth-width)/2;
                                var top = (document.body.clientHeight-height)/2;
                            }
                            var factureWindow = window.open('','facture','menubar=yes, scrollbars=yes, top='+top+', left='+left+', width='+width+', height='+height+'');
                            factureText = "Facture pour : " + prenom + " " + nom;
                            factureWindow.document.write(factureText);
                         } 
                    ]]>
                </script>
                <link rel="stylesheet" type="text/css" href="Style.css"/>
            </head>
            <body>
                <xsl:apply-templates select="//infirmier[@id=$destinedId]"/>
            </body>
        </html>
    </xsl:template>

    <xsl:template match="infirmier">
        <xsl:variable name="nbPatients" select="count(//patient[visites/visite[@date=$dateJour and @intervenant=$destinedId]])"/>
        <header>
            <div class="panel perso">
                <div class="block">
                    <div class="info">
                        <h1>
                            Bonjour <xsl:value-of select="prenom"/>
                        </h1>
                        <h2>
                            <xsl:if test="$nbPatients &gt; 1">
                                Aujourd'hui vous avez <xsl:value-of select="$nbPatients"/> patients.
                            </xsl:if>
                            <xsl:if test="$nbPatients = 1">
                                Aujourd'hui vous avez <xsl:value-of select="$nbPatients"/> patient.
                            </xsl:if>
                            <xsl:if test="$nbPatients = 0">
                                Aujourd'hui vous n'avez pas de patient.
                            </xsl:if>
                        </h2>
                    </div>
                    <div class="photo">
                        <xsl:variable name="srcAvatar" select="photo"/>
                        <img class="img">
                            <xsl:attribute name="src"><xsl:value-of select="$srcAvatar"/></xsl:attribute>
                        </img>
                    </div>
                </div>
            </div>
        </header>
        <section>
            <div class="main">
                <xsl:if test="$nbPatients != 0">
                    <div class="panel formation">
                        <h1 class="titre">Patients</h1>
                        <xsl:apply-templates select="//patient[visites/visite[@date=$dateJour and @intervenant=$destinedId]]"/>
                    </div>
                </xsl:if>
                
                <xsl:if test="$nbPatients = 0">
                    <div class="panel formation">
                        <h1 class="titre">Patients</h1>
                        Aucun Patients Aujourd'hui
                    </div>
                </xsl:if>
            </div>
        </section>
        <footer>
        </footer>
    </xsl:template>

    <xsl:template match="patient">
        <div class="patient panel">
            <div class="block">
                <div class="tabPatient left">
                    <h3>Informations Personelles : </h3>
                    <table>
                        <tr>
                            <td>Nom : </td>
                            <td>
                                <xsl:value-of select="nom"/>&#160;
                                <xsl:value-of select="prenom"/>
                            </td>
                        </tr>
                        <xsl:variable name="nbEtage" select="count(adresse/etage)"/>
                        <xsl:if test="$nbEtage != 0">
                            <tr>
                                <td>Etage : </td>
                                <td>
                                    <xsl:value-of select="adresse/etage"/>
                                </td>
                            </tr>
                        </xsl:if>
                        <xsl:variable name="nbNum" select="count(adresse/numero)"/>
                        <xsl:if test="$nbNum != 0">
                            <tr>
                                <td>Num. : </td>
                                <td>
                                    <xsl:value-of select="adresse/numero"/>
                                </td>
                            </tr>
                        </xsl:if>

                        <tr>
                            <td>Rue : </td>
                            <td>
                                <xsl:value-of select="adresse/rue"/>
                            </td>
                        </tr>
                        <tr>
                            <td>Code Postal : </td>
                            <td>
                                <xsl:value-of select="adresse/codePostal"/>
                            </td>
                        </tr>
                        <tr>
                            <td>Ville : </td>
                            <td>
                                <xsl:value-of select="adresse/ville"/>
                            </td>
                        </tr>
                    </table>
                </div>
                <div class="map left">
                    <h3>[WIP] Situation Géographique :</h3>
                    <div class="divMap">
                        <img src="map.jpg"/>
                    </div>
                </div>
            </div>
            <xsl:apply-templates select="visites"/>
            <br/>
            <center>
                <input type="button" value="Facture">
                    <xsl:attribute name="onclick">
                        openFacture('<xsl:value-of select="nom"/>',
                        '<xsl:value-of select="prenom"/>',
                        '<xsl:value-of select="visites/visite/acte"/>')
                    </xsl:attribute>
                </input>
            </center>
        </div>
    </xsl:template>

    <xsl:template match="visites">
        <h3>Visites : </h3>
        <div class="visite">
            <table>
                <tr>
                    <td>Date</td>
                    <td>Acte(s)</td>
                </tr>
                <xsl:apply-templates select="visite"/>
            </table>
        </div>
    </xsl:template>

    <xsl:template match="visite">
        <tr>
            <td>
                <xsl:value-of select="@date"/>
            </td>
            <td>
                <xsl:apply-templates select="acte"/>
            </td>
        </tr>
    </xsl:template>

    <xsl:template match="acte">
        <xsl:variable name="id" select="@id"/>
        <xsl:variable name="actes" select="document('actes.xml', /)/ngap"/>
        <xsl:value-of select="$actes/actes/acte[@id=$id]/text()"/>
        <br/>
    </xsl:template>

    <xsl:template match="text()"/>

</xsl:stylesheet>